#include "cpu.h"
#include "status_macros.h"
#include "absl/strings/str_cat.h"
#include "absl/status/statusor.h"
#include "lib/logic/wire.h"
#include "lib/branch_cmp/branch_cmp.h"
#include <iostream>

namespace riscv_emu {

  namespace {

    absl::StatusOr<uint32_t> GetRegister(uint32_t registers[32], const size_t at_index) {
      if (at_index > 31) {
        const std::string error_str = absl::StrCat("Invalid register index ", at_index);
        return absl::OutOfRangeError(error_str);
      }
      return registers[at_index];
    }

    absl::Status SetRegister(uint32_t registers[32], const size_t at_index, const uint32_t val) {
      if (at_index > 31) {
        const std::string error_str = absl::StrCat("Invalid register index 0x", at_index);
        return absl::OutOfRangeError(error_str);
      }
      registers[at_index] = val;
      return absl::OkStatus();
    }

    void PrintRegisters(uint32_t registers[32]) {
      VLOG(1) << "Register debug info:";
      for (int i = 0; i < 32; ++i) {
        if (i < 10) {
          VLOG(1) << "0x0" << std::dec << i << ": " << std::hex << registers[i] << std::endl;
        } else {
          VLOG(2) << "0x" << std::dec << i << ": " << std::hex << registers[i] << std::endl;
        }
        
      }
    }


  }  // namespace

Cpu::Cpu() {
  imem_.SetAccessType(memory::AccessType::kWord);
  if (!imem_.Flash("/tmp/progs/foo.o").ok()) {
    LOG(FATAL) << "Something happened";
  }

}

absl::Status Cpu::Fetch() {
  switch (decoder_.GetPcSel()) {
   case decoder::PcSel::kPcPlus4:
    pc_ += 4;
    break;
   case decoder::PcSel::kAluOut:
    pc_ = alu_out_;
    break;
   default:
    return absl::InternalError("Invalid PC select");
  }

  VLOG(1) << "PC: 0x" << std::hex << pc_;
  absl::StatusOr<uint32_t> instr = imem_.Read(pc_);
  if (!instr.ok()) {
    if (absl::IsOutOfRange(instr.status())) {
    // TODO: Raise exception and loop PC to addr. 0x0.
    } else {
      return instr.status();
    }
  }
  VLOG(1) << "Instruction: 0x" << std::hex << *instr;
  instr_ = *instr;
  return absl::OkStatus();
}

absl::Status Cpu::Decode() {
  absl::Status decoder_status = decoder_.Decode(instr_);
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
  ASSIGN_OR_RETURN(const uint32_t rs1_out, GetRegister(registers_, decoder_.GetRs1()));
  ASSIGN_OR_RETURN(const uint32_t rs2_out, GetRegister(registers_, decoder_.GetRs2()));

  switch (decoder_.GetASel()) {
   case decoder::ASel::kRegOut:
    a_out_ = rs1_out;
    break;
   case decoder::ASel::kPcOut:
    a_out_ = pc_;
    break;
   case decoder::ASel::kNone:
    break;
   default:
    return absl::InternalError("Invalid A-sel");
    break;
  }
  absl::StatusOr<uint32_t> immediate;
  switch (decoder_.GetBSel()) {
   case decoder::BSel::kRegOut:
    b_out_ = rs2_out;
    break;
   case decoder::BSel::kImmOut:
    immediate = imm::DecodeImm(decoder_.GetImmSel(), instr_);
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
  ASSIGN_OR_RETURN(const uint32_t rs1_out, GetRegister(registers_, decoder_.GetRs1()));
  ASSIGN_OR_RETURN(const uint32_t rs2_out, GetRegister(registers_, decoder_.GetRs2()));
  const branch::ComparisonResult res = branch::DoBranchComp(decoder_.IsBranchUnsigned(), rs1_out, rs2_out);
  RETURN_IF_ERROR(decoder_.SetBranchComp(res));

  ASSIGN_OR_RETURN(alu_out_, alu_.DoOp(decoder_.GetAluSel(), a_out_, b_out_));
  return absl::OkStatus();
}

absl::Status Cpu::Memory() {
  dmem_.SetAccessType(decoder_.GetMemSel());
  ASSIGN_OR_RETURN(const uint32_t rs2_out, GetRegister(registers_, decoder_.GetRs2()));
  absl::StatusOr<uint32_t> mem_out;
  absl::Status mem_write_status;
  switch (decoder_.GetMemOp()) {
   case decoder::MemOp::kNone:
    break;
   case decoder::MemOp::kRead:
    mem_out = dmem_.Read(alu_out_);
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
    mem_write_status = dmem_.Write(alu_out_, rs2_out);
    if (!mem_write_status.ok()) {
      if (absl::IsOutOfRange(mem_write_status)) {
        // TODO: Raise a bus error exception.
        break;
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
    RETURN_IF_ERROR(SetRegister(registers_, decoder_.GetRd(), alu_out_));
    break;
   case decoder::WbSel::kMemOut:
    RETURN_IF_ERROR(SetRegister(registers_, decoder_.GetRd(), mem_out_));
    break;
   case decoder::WbSel::kPcPlus4:
    RETURN_IF_ERROR(SetRegister(registers_, decoder_.GetRd(), pc_ + 4));
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
