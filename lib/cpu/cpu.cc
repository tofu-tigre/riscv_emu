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

    absl::Status SetRegister(logic::Register registers[32], const size_t at_index, const logic::Wire val) {
      if (at_index > 31) {
        return absl::OutOfRangeError("Cannot access register at index > 31");
      }
      registers[at_index].SetIn(val);
      return absl::OkStatus();
    }

    void PrintRegisters(const logic::Register registers[32]) {
      VLOG(4) << "Register debug info:";
      for (int i = 0; i < 32; ++i) {
        if (i < 10) {
          VLOG(4) << "0x0" << std::dec << i << ": " << std::hex << registers[i].GetOut().GetUnsigned() << std::endl;
        } else {
          VLOG(5) << "0x" << std::dec << i << ": " << std::hex << registers[i].GetOut().GetUnsigned() << std::endl;
        }
        
      }
    }


  }  // namespace

Cpu::Cpu() {
  pc_.SetIn(logic::Wire(0x800U - 0x4U));
  imem_.SetAccessType(memory::AccessType::kWord);
  if (!imem_.Flash("/tmp/progs/fib.o", 0x800).ok()) {
    LOG(FATAL) << "Something happened";
  }
  LOG(INFO) << "0x800: " << std::hex << imem_.Read(0x8000U).value().GetUnsigned();
  // imem_.Write(0x8000U, logic::Wire(0x00a00093)).IgnoreError();  // addi x1, x0, 10
  // imem_.Write(0x8004U, logic::Wire(0xfff08093)).IgnoreError();  // addi x1, x1, -1
  // imem_.Write(0x8008U, logic::Wire(0xfe009ee3)).IgnoreError();  // bne x1, x0, -4
  // imem_.Write(0x800cU, logic::Wire(0xdeadc137)).IgnoreError();  // lui x2, 0xdeadc
  // imem_.Write(0x8010U, logic::Wire(0xeef10113)).IgnoreError();  // addi x2, x2, 0xeef
  // imem_.Write(0x8014U, logic::Wire(0x004003ef)).IgnoreError();  // jal x7, 4
  
  

}

absl::Status Cpu::Fetch() {
  switch (decoder_.GetPcSel()) {
   case PcSel::kPcPlus4:
    pc_.SetIn(logic::Wire(pc_.GetOut().GetUnsigned() + 4));
    break;
   case PcSel::kAluOut:
    pc_.SetIn(alu_out_);
    break;
   default:
    return absl::InternalError("Invalid PC select detected");
  }

  VLOG(4) << "Fetching next instruction from PC: " << pc_.GetOut();
  ASSIGN_OR_RETURN(logic::Wire instr, imem_.Read(pc_.GetOut().GetUnsigned()));
  instr_.SetIn(instr);

  return absl::OkStatus();
}

absl::Status Cpu::Decode() {
  VLOG(4) << "Decoding instruction: " << std::hex << instr_.GetOut().GetUnsigned();
  RETURN_IF_ERROR(decoder_.Decode(instr_.GetOut()));

  if(decoder_.GetESel() == ESel::kEBreak) {
    power_is_on_ = false;
  }

  ASSIGN_OR_RETURN(const logic::Wire rs1_out, GetRegister(registers_, decoder_.GetRs1().GetUnsigned()));
  ASSIGN_OR_RETURN(const logic::Wire rs2_out, GetRegister(registers_, decoder_.GetRs2().GetUnsigned()));

  switch (decoder_.GetASel()) {
   case ASel::kRegOut:
    a_out_ = rs1_out;
    break;
   case ASel::kPcOut:
    a_out_ = pc_.GetOut();
    break;
   default:
    // return absl::InternalError("Invalid A-sel detected");
    break;
  }
  absl::StatusOr<logic::Wire> immediate;
  switch (decoder_.GetBSel()) {
   case BSel::kRegOut:
    b_out_ = rs2_out;
    break;
   case BSel::kImmOut:
    immediate = imm::DecodeImm(decoder_.GetImmSel(), instr_.GetOut());
    if (!immediate.ok()) {
      return immediate.status();
    }
    b_out_ = *immediate;
    break;
   default:
    return absl::InternalError("Invalid A-sel detected");
  }
  return absl::OkStatus();
}

absl::Status Cpu::Execute() {
  VLOG(4) << "Executing instruction: " << instr_.GetOut();

  ASSIGN_OR_RETURN(const logic::Wire rs1_out, GetRegister(registers_, decoder_.GetRs1().GetUnsigned()));
  ASSIGN_OR_RETURN(const logic::Wire rs2_out, GetRegister(registers_, decoder_.GetRs2().GetUnsigned()));
  const branch::ComparisonResult res = branch::DoBranchComp(decoder_.IsBranchUnsigned(), rs1_out, rs2_out);
  RETURN_IF_ERROR(decoder_.SetBranchComp(res));

  ASSIGN_OR_RETURN(alu_out_, alu_.DoOp(decoder_.GetAluSel(), a_out_, b_out_));
  VLOG(5) << "Alu a-out is: " << a_out_;
  VLOG(5) << "Alu b-out is: " << b_out_;
  VLOG(5) << "Alu output is: " << alu_out_;
  return absl::OkStatus();
}

absl::Status Cpu::Memory() {
  dmem_.SetAccessType(decoder_.GetMemSel());
  ASSIGN_OR_RETURN(const logic::Wire rs2_out, GetRegister(registers_, decoder_.GetRs2().GetUnsigned()));
  absl::StatusOr<logic::Wire> mem_out;
  absl::Status mem_write_status;
  switch (decoder_.GetMemOp()) {
   case MemOp::kNone:
    break;
   case MemOp::kRead:
    mem_out = dmem_.Read(alu_out_.GetUnsigned());
    if (!mem_out.ok()) {
      return mem_out.status();
    }
    mem_out_ = *mem_out;
    break;
   case MemOp::kWrite:
    mem_write_status = dmem_.Write(alu_out_.GetUnsigned(), rs2_out);
    if (!mem_write_status.ok()) {
      return mem_write_status;
    }
    break;
   default:
    return absl::InternalError("Invalid memory operation detected");
  }
  return absl::OkStatus();
}

absl::Status Cpu::Writeback() {
  VLOG(4) << "Doing writeback for instruction: " << instr_.GetOut();
  if (!decoder_.GetRegWriteEn()) {
    return absl::OkStatus();
  }
  switch (decoder_.GetWbSel()) {
   case WbSel::kAluOut:
    VLOG(5) << "Writing to dest. register 0x" << decoder_.GetRd().GetUnsigned() << " from alu output: " << alu_out_;
    RETURN_IF_ERROR(SetRegister(registers_, decoder_.GetRd().GetUnsigned(), alu_out_));
    break;
   case WbSel::kMemOut:
    RETURN_IF_ERROR(SetRegister(registers_, decoder_.GetRd().GetUnsigned(), mem_out_));
    break;
   case WbSel::kPcPlus4:
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
