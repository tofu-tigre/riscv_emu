#include "cpu.h"
#include "status_macros.h"
#include "absl/status/statusor.h"
#include "lib/logic/wire.h"
#include "lib/branch_cmp/branch_cmp.h"
#include <iostream>

namespace riscv_emu {

  namespace {

    absl::StatusOr<logic::Wire> GetRegister(logic::Register registers[32], const size_t at_index) {
      if (at_index > 31) {
        return absl::OutOfRangeError("Cannot access register at index > 31");
      }
      return registers[at_index].GetOut();
    }

    absl::StatusOr<logic::Wire> GetRegister(logic::Register registers[32], const logic::Wire at_index) {
      if (at_index.GetUnsigned() > 31) {
        return absl::OutOfRangeError("Cannot access register at index > 31");
      }
      return registers[at_index.GetUnsigned()].GetOut();
    }

    absl::Status SetRegister(logic::Register registers[32], const size_t at_index, const logic::Wire val) {
      if (at_index > 31) {
        return absl::OutOfRangeError("Cannot access register at index > 31");
      }
      registers[at_index].SetIn(val);
      return absl::OkStatus();
    }

    void PrintRegisters(const logic::Register registers[32]) {
      VLOG(1) << "Register debug info:";
      for (int i = 0; i < 32; ++i) {
        if (i < 10) {
          VLOG(1) << "0x0" << std::dec << i << ": " << std::hex << registers[i].GetOut().GetUnsigned() << std::endl;
        } else {
          VLOG(2) << "0x" << std::dec << i << ": " << std::hex << registers[i].GetOut().GetUnsigned() << std::endl;
        }
        
      }
    }


  }  // namespace

Cpu::Cpu() {
  pc_.SetIn(logic::Wire(0x800U - 0x4U));
  imem_.SetAccessType(memory::AccessType::kWord);
  if (!imem_.Flash("/tmp/progs/foo.bin", 0x800).ok()) {
    LOG(FATAL) << "Something happened";
  }
  // imem_.Write(0x8000U, logic::Wire(0x00a00093)).IgnoreError();  // addi x1, x0, 10
  // imem_.Write(0x8004U, logic::Wire(0xfff08093)).IgnoreError();  // addi x1, x1, -1
  // imem_.Write(0x8008U, logic::Wire(0xfe009ee3)).IgnoreError();  // bne x1, x0, -4
  // imem_.Write(0x800cU, logic::Wire(0xdeadc137)).IgnoreError();  // lui x2, 0xdeadc
  // imem_.Write(0x8010U, logic::Wire(0xeef10113)).IgnoreError();  // addi x2, x2, 0xeef
  // imem_.Write(0x8014U, logic::Wire(0x004003ef)).IgnoreError();  // jal x7, 4
  
  

}

absl::Status Cpu::Fetch() {
  switch (decoder_.GetPcSel()) {
   case decoder::PcSel::kPcPlus4:
    pc_.SetIn(logic::Wire(pc_.GetOut().GetUnsigned() + 4));
    break;
   case decoder::PcSel::kAluOut:
    pc_.SetIn(alu_out_);
    break;
   default:
    return absl::InternalError("Invalid PC select");
  }

  VLOG(1) << "PC: 0x" << std::hex << pc_.GetOut().GetUnsigned();
  absl::StatusOr<logic::Wire> instr = imem_.Read(pc_.GetOut().GetUnsigned());
  if (!instr.ok()) {
    if (absl::IsOutOfRange(instr.status())) {
    // TODO: Raise exception and loop PC to addr. 0x0.
    } else {
      return instr.status();
    }
  }
  VLOG(1) << "Instruction: 0x" << std::hex << instr->GetUnsigned();
  instr_.SetIn(*instr);
  return absl::OkStatus();
}

absl::Status Cpu::Decode() {
  absl::Status decoder_status = decoder_.Decode(instr_.GetOut());
  if (!decoder_status.ok()) {
    if (absl::IsInvalidArgument(decoder_status)) {
      // TODO: Raise exception.
      // But for now, exit.
      return decoder_status;
    } else {
      return decoder_status;
    }
  }
  if(decoder_.GetESel() == decoder::ESel::kEBreak) {
    power_is_on_ = false;
  }
  ASSIGN_OR_RETURN(const logic::Wire rs1_out, GetRegister(registers_, decoder_.GetRs1()));
  ASSIGN_OR_RETURN(const logic::Wire rs2_out, GetRegister(registers_, decoder_.GetRs2()));

  switch (decoder_.GetASel()) {
   case decoder::ASel::kRegOut:
    a_out_ = rs1_out;
    break;
   case decoder::ASel::kPcOut:
    a_out_ = pc_.GetOut();
    break;
   case decoder::ASel::kNone:
    break;
   default:
    return absl::InternalError("Invalid A-sel");
    break;
  }
  absl::StatusOr<logic::Wire> immediate;
  switch (decoder_.GetBSel()) {
   case decoder::BSel::kRegOut:
    b_out_ = rs2_out;
    break;
   case decoder::BSel::kImmOut:
    immediate = imm::DecodeImm(decoder_.GetImmSel(), instr_.GetOut());
    if (!immediate.ok()) {
      return immediate.status();
    }
    b_out_ = *immediate;
    break;
   case decoder::BSel::kNone:
    break;
   default:
    return absl::InternalError("Invalid B-sel");
  }
  return absl::OkStatus();
}

absl::Status Cpu::Execute() {
  ASSIGN_OR_RETURN(const logic::Wire rs1_out, GetRegister(registers_, decoder_.GetRs1()));
  ASSIGN_OR_RETURN(const logic::Wire rs2_out, GetRegister(registers_, decoder_.GetRs2()));
  const branch::ComparisonResult res = branch::DoBranchComp(decoder_.IsBranchUnsigned(), rs1_out, rs2_out);
  RETURN_IF_ERROR(decoder_.SetBranchComp(res));

  ASSIGN_OR_RETURN(alu_out_, alu_.DoOp(decoder_.GetAluSel(), a_out_, b_out_));
  return absl::OkStatus();
}

absl::Status Cpu::Memory() {
  dmem_.SetAccessType(decoder_.GetMemSel());
  ASSIGN_OR_RETURN(const logic::Wire rs2_out, GetRegister(registers_, decoder_.GetRs2()));
  absl::StatusOr<logic::Wire> mem_out;
  absl::Status mem_write_status;
  switch (decoder_.GetMemOp()) {
   case decoder::MemOp::kNone:
    break;
   case decoder::MemOp::kRead:
    mem_out = dmem_.Read(alu_out_.GetUnsigned());
    if (!mem_out.ok()) {
      if (absl::IsOutOfRange(mem_out.status())) {
        // TODO: Raise a bus error exception.
        break;
      }
      return mem_out.status();
    }
    mem_out_ = *mem_out;
    break;
   case decoder::MemOp::kWrite:
    mem_write_status = dmem_.Write(alu_out_.GetUnsigned(), rs2_out);
    if (!mem_write_status.ok()) {
      if (absl::IsOutOfRange(mem_write_status)) {
        // TODO: Raise a bus error exception.
      }
      return mem_write_status;
    }
    break;
   default:
    return absl::InternalError("Invalid memory operation");
  }
  return absl::OkStatus();
}

absl::Status Cpu::Writeback() {
  if (!decoder_.GetRegWriteEn()) {
    return absl::OkStatus();
  }
  switch (decoder_.GetWbSel()) {
   case decoder::WbSel::kNone:
   case decoder::WbSel::kAluOut:
    RETURN_IF_ERROR(SetRegister(registers_, decoder_.GetRd().GetUnsigned(), alu_out_));
    break;
   case decoder::WbSel::kMemOut:
    RETURN_IF_ERROR(SetRegister(registers_, decoder_.GetRd().GetUnsigned(), mem_out_));
    break;
   case decoder::WbSel::kPcPlus4:
    RETURN_IF_ERROR(SetRegister(registers_, decoder_.GetRd().GetUnsigned(), logic::Wire(pc_.GetOut().GetUnsigned() + 4)));
    break;
   default:
    return absl::InternalError("Invalid writeback");
  }
  return absl::OkStatus();
}

absl::Status Cpu::Boot() {
  power_is_on_ = true;

  while (power_is_on_) {
    RETURN_IF_ERROR(Fetch());
    RETURN_IF_ERROR(Decode());
    RETURN_IF_ERROR(Execute());
    RETURN_IF_ERROR(Memory());
    RETURN_IF_ERROR(Writeback());
    PrintRegisters(registers_);
  }
  return absl::OkStatus();
}

}  // namespace riscv_emu
