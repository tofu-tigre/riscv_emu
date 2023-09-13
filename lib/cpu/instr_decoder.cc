#include "instr_decoder.h"
#include "status_macros.h"
#include "glog/logging.h"

namespace riscv_emu::decoder {

absl::Status InstrDecoder::DecodeRTypeInstr() {
  VLOG(5) << "Decoding R-type instruction";
  // Set register selects.
  ASSIGN_OR_RETURN(const uint32_t rs1, instr_.GetRs1());
  ASSIGN_OR_RETURN(const uint32_t rs2, instr_.GetRs2());
  ASSIGN_OR_RETURN(const uint32_t rd, instr_.GetRd());
  rs1_sel_ = logic::Wire(rs1);
  rs2_sel_ = logic::Wire(rs2);
  rd_sel_ = logic::Wire(rd);

  // Set selects.
  op_ = logic::Opcode::kRType;
  b_sel_ = BSel::kRegOut;
  a_sel_ = ASel::kRegOut;
  wb_sel_ = WbSel::kAluOut;
  pc_sel_ = PcSel::kPcPlus4;
  reg_write_en_ = (rd != 0) ? true : false;
  mem_op_ = MemOp::kNone;

  // Set ALU.
  ASSIGN_OR_RETURN(const uint32_t func3, instr_.GetFunc3());
  ASSIGN_OR_RETURN(const uint32_t func7, instr_.GetFunc7());
  switch (func3) {
   case static_cast<uint32_t>(AluOp::kAdd):
   // case static_cast<uint32_t>(AluOp::kSub): not needed but left for clarity.
    // Special case: `AluOp::kAdd` and `AluOp::kSub` have the
    // same func3 encoding, so must also peek at func7.
    if (func7 == /*add instr. func7 encoding=*/0) {
      alu_sel_ = AluOp::kAdd;
    } else if (func7 == /*sub instr. func7 encoding=*/0b0100000) {
      alu_sel_ = AluOp::kSub;
    } else {
      return absl::InvalidArgumentError("Invalid func7 encoding");
    }
    break;
   case static_cast<uint32_t>(AluOp::kOr):
    alu_sel_ = AluOp::kOr;
    break;
   case static_cast<uint32_t>(AluOp::kAnd):
    alu_sel_ = AluOp::kAnd;
    break;
   case static_cast<uint32_t>(AluOp::kXor):
    alu_sel_ = AluOp::kXor;
    break;
   case static_cast<uint32_t>(AluOp::kSll):
    alu_sel_ = AluOp::kSll;
    break;
   case static_cast<uint32_t>(AluOp::kSra):
   // case static_cast<uint32_t>(AluOp::kSrl): not needed but left for clarity.
    // Special case: `AluOp::kSra` and `AluOp::kSrl` have the
    // same func3 encoding, so must also peek at func7.
    if (func7 == /*srl instr. func7 encoding=*/0) {
      alu_sel_ = AluOp::kSrl;
    } else if (func7 == /*sra instr. func7 encoding=*/0b0100000) {
      alu_sel_ = AluOp::kSra;
    } else {
      return absl::InvalidArgumentError("Invalid func7 encoding");
    }
    break;
   default:
    return absl::InvalidArgumentError("Invalid func3 encoding");
  }
  return absl::OkStatus();
}

absl::Status InstrDecoder::DecodeITypeInstr() {
  VLOG(5) << "Decoding I-type instruction";
  // Set register selects.
  ASSIGN_OR_RETURN(const uint32_t rs1, instr_.GetRs1());
  ASSIGN_OR_RETURN(const uint32_t rd, instr_.GetRd());
  rs1_sel_ = logic::Wire(rs1);
  rd_sel_ = logic::Wire(rd);

  // Set selects.
  op_ = logic::Opcode::kIType;
  b_sel_ = BSel::kImmOut;
  a_sel_ = ASel::kRegOut;
  wb_sel_ = WbSel::kAluOut;
  pc_sel_ = PcSel::kPcPlus4;
  reg_write_en_ = (rd != 0) ? true : false;
  imm_sel_ = imm::ImmSel::kIType;
  mem_op_ = MemOp::kNone;


  // Set ALU.
  ASSIGN_OR_RETURN(const uint32_t func3, instr_.GetFunc3());
  ASSIGN_OR_RETURN(const uint32_t func7, instr_.GetFunc7());
  switch (func3) {
   case static_cast<uint32_t>(AluOp::kAdd):
    alu_sel_ = AluOp::kAdd;
    break;
   case static_cast<uint32_t>(AluOp::kOr):
    alu_sel_ = AluOp::kOr;
    break;
   case static_cast<uint32_t>(AluOp::kAnd):
    alu_sel_ = AluOp::kAnd;
    break;
   case static_cast<uint32_t>(AluOp::kXor):
    alu_sel_ = AluOp::kXor;
    break;
   case static_cast<uint32_t>(AluOp::kSll):
    alu_sel_ = AluOp::kSll;
    break;
   case static_cast<uint32_t>(AluOp::kSra):
   // case static_cast<uint32_t>(AluOp::kSrl): not needed but left for clarity.
    // Special case: `AluOp::kSra` and `AluOp::kSrl` have the
    // same func3 encoding, so must also peek at func7.
    if (func7 == /*srl instr. func7 encoding=*/0) {
      alu_sel_ = AluOp::kSrl;
    } else if (func7_imm == /*sra instr. func7 encoding=*/0b0100000) {
      alu_sel_ = AluOp::kSra;
    } else {
      return absl::InvalidArgumentError("Invalid func7 encoding");
    }
    break;
   default:
    return absl::InvalidArgumentError("Invalid func3 encoding");
  }
  return absl::OkStatus();
}

absl::Status InstrDecoder::DecodeILTypeInstr() {
  VLOG(5) << "Decoding I-type instruction (load)";
  // Set register selects.
  ASSIGN_OR_RETURN(const uint32_t rs1, instr_.GetRs1());
  ASSIGN_OR_RETURN(const uint32_t rd, instr_.GetRd());
  rs1_sel_ = logic::Wire(rs1);
  rd_sel_ = logic::Wire(rd);

  // Set selects.
  op_ = logic::Opcode::kLType;
  b_sel_ = BSel::kImmOut;
  a_sel_ = ASel::kRegOut;
  wb_sel_ = WbSel::kMemOut;
  pc_sel_ = PcSel::kPcPlus4;
  reg_write_en_ = (rd != 0) ? true : false;
  imm_sel_ = imm::ImmSel::kIType;
  alu_sel_ = AluOp::kAdd;
  mem_op_ = MemOp::kRead;

  // Set ALU.
  ASSIGN_OR_RETURN(const uint32_t func3, instr_.GetFunc3());
  switch (func3) {
   case static_cast<uint32_t>(memory::AccessType::kByte):
    mem_sel_ = memory::AccessType::kByte;
    break;
   case static_cast<uint32_t>(memory::AccessType::kHalfword):
    mem_sel_ = memory::AccessType::kHalfword;
    break;
   case static_cast<uint32_t>(memory::AccessType::kWord):
    mem_sel_ = memory::AccessType::kWord;
    break;
   case static_cast<uint32_t>(memory::AccessType::kByteUnsigned):
    mem_sel_ = memory::AccessType::kByteUnsigned;
    break;
   case static_cast<uint32_t>(memory::AccessType::kHalfwordUnsigned):
    mem_sel_ = memory::AccessType::kHalfwordUnsigned;
    break;
   default:
    return absl::InvalidArgumentError("Invalid func3 encoding");
  }
  return absl::OkStatus();
}

absl::Status InstrDecoder::DecodeSTypeInstr() {
  VLOG(5) << "Decoding S-type instruction";
  // Set register selects.
  ASSIGN_OR_RETURN(const uint32_t rs1, instr_.GetRs1());
  ASSIGN_OR_RETURN(const uint32_t rs2, instr_.GetRs2());
  rs1_sel_ = logic::Wire(rs1);
  rs2_sel_ = logic::Wire(rs2);

  // Set selects.
  op_ = logic::Opcode::kSType;
  b_sel_ = BSel::kImmOut;
  a_sel_ = ASel::kRegOut;
  pc_sel_ = PcSel::kPcPlus4;
  reg_write_en_ = false;
  imm_sel_ = imm::ImmSel::kSType;
  alu_sel_ = AluOp::kAdd;
  mem_op_ = MemOp::kWrite;

  // Set ALU.
  ASSIGN_OR_RETURN(const uint32_t func3, instr_.GetFunc3());
  switch (func3) {
   case static_cast<uint32_t>(memory::AccessType::kByte):
    mem_sel_ = memory::AccessType::kByte;
    break;
   case static_cast<uint32_t>(memory::AccessType::kHalfword):
    mem_sel_ = memory::AccessType::kHalfword;
    break;
   case static_cast<uint32_t>(memory::AccessType::kWord):
    mem_sel_ = memory::AccessType::kWord;
    break;
   default:
    return absl::InvalidArgumentError("Invalid func3 encoding");
  }
  return absl::OkStatus();
}

absl::Status InstrDecoder::DecodeBTypeInstr() {
  VLOG(5) << "Decoding B-type instruction";
  // Set register selects.
  ASSIGN_OR_RETURN(const uint32_t rs1, instr_.GetRs1());
  ASSIGN_OR_RETURN(const uint32_t rs2, instr_.GetRs2());
  rs1_sel_ = logic::Wire(rs1);
  rs2_sel_ = logic::Wire(rs2);

  // Set selects.
  op_ = logic::Opcode::kBType;
  b_sel_ = BSel::kImmOut;
  a_sel_ = ASel::kPcOut;
  is_branch_unsigned_ = true;
  pc_sel_ = PcSel::kPcPlus4;
  reg_write_en_ = false;
  imm_sel_ = imm::ImmSel::kBType;
  alu_sel_ = AluOp::kAdd;
  mem_op_ = MemOp::kNone;
  return absl::OkStatus();
}

absl::Status InstrDecoder::SetBranchComp(const branch::ComparisonResult result) {
  if (op_ != logic::Opcode::kBType) {
    return absl::OkStatus();
  }
  const uint32_t func3 = instr_.GetFunc3().value();
  switch (func3) {
   case static_cast<uint32_t>(branch::ComparisonType::kEqual):
    pc_sel_ = result.branch_eq_ ? PcSel::kAluOut : pc_sel_;
    return absl::OkStatus();
   case static_cast<uint32_t>(branch::ComparisonType::kNotEqual):
    pc_sel_ = (!result.branch_eq_) ? PcSel::kAluOut : pc_sel_;
    return absl::OkStatus();
   case static_cast<uint32_t>(branch::ComparisonType::kLessThan):
   case static_cast<uint32_t>(branch::ComparisonType::kLessThanUnsigned):
    pc_sel_ = result.branch_lt_ ? PcSel::kAluOut : pc_sel_;
    return absl::OkStatus();
   case static_cast<uint32_t>(branch::ComparisonType::kGreaterThanOrEqual):
   case static_cast<uint32_t>(branch::ComparisonType::kGreaterThanOrEqualUnsigned):
    pc_sel_ = (result.branch_eq_ || !result.branch_lt_) ? PcSel::kAluOut : pc_sel_;
    return absl::OkStatus();
   default:
    return absl::InvalidArgumentError("b-type instruction has invalid func3 encoding");
  }
}

absl::Status InstrDecoder::DecodeLuiTypeInstr() {
  VLOG(5) << "Decoding U-type instruction";
  // Set register selects.
  ASSIGN_OR_RETURN(const uint32_t rd, instr_.GetRd());
  rd_sel_ = logic::Wire(rd);

  // Set selects.
  op_ = logic::Opcode::kLuiType;
  b_sel_ = BSel::kImmOut;
  pc_sel_ = PcSel::kPcPlus4;
  reg_write_en_ = (rd != 0) ? true : false;
  wb_sel_ = WbSel::kAluOut;
  imm_sel_ = imm::ImmSel::kUType;
  alu_sel_ = AluOp::kBCopy;
  mem_op_ = MemOp::kNone;
  return absl::OkStatus();
}

absl::Status InstrDecoder::DecodeAuiPcTypeInstr() {
  VLOG(5) << "Decoding U-type instruction";
  // Set register selects.
  ASSIGN_OR_RETURN(const uint32_t rd, instr_.GetRd());
  rd_sel_ = logic::Wire(rd);

  // Set selects.
  op_ = logic::Opcode::kAuiPcType;
  a_sel_ = ASel::kPcOut;
  b_sel_ = BSel::kImmOut;
  pc_sel_ = PcSel::kPcPlus4;
  reg_write_en_ = (rd != 0) ? true : false;
  wb_sel_ = WbSel::kAluOut;
  imm_sel_ = imm::ImmSel::kUType;
  alu_sel_ = AluOp::kBCopy;
  mem_op_ = MemOp::kNone;
  return absl::OkStatus();
}

absl::Status InstrDecoder::DecodeJalTypeInstr() {
  VLOG(5) << "Decoding J-type instruction";
  // Set register selects.
  ASSIGN_OR_RETURN(const uint32_t rd, instr_.GetRd());
  rd_sel_ = logic::Wire(rd);

  // Set selects.
  op_ = logic::Opcode::kJalType;
  a_sel_ = ASel::kPcOut;
  b_sel_ = BSel::kImmOut;
  pc_sel_ = PcSel::kAluOut;
  reg_write_en_ = (rd != 0) ? true : false;
  wb_sel_ = WbSel::kPcPlus4;
  imm_sel_ = imm::ImmSel::kJType;
  alu_sel_ = AluOp::kAdd;
  mem_op_ = MemOp::kNone;
  return absl::OkStatus();
}

absl::Status InstrDecoder::DecodeJalrTypeInstr() {
  VLOG(5) << "Decoding J-type instruction";
  // Set register selects.
  ASSIGN_OR_RETURN(const uint32_t rs1, instr_.GetRs1());
  ASSIGN_OR_RETURN(const uint32_t rd, instr_.GetRd());
  rs1_sel_ = logic::Wire(rs1);
  rd_sel_ = logic::Wire(rd);

  // Set selects.
  op_ = logic::Opcode::kJalrType;
  a_sel_ = ASel::kRegOut;
  b_sel_ = BSel::kImmOut;
  pc_sel_ = PcSel::kAluOut;
  reg_write_en_ = (rd != 0) ? true : false;
  wb_sel_ = WbSel::kPcPlus4;
  imm_sel_ = imm::ImmSel::kIType;
  alu_sel_ = AluOp::kAdd;
  mem_op_ = MemOp::kNone;
  return absl::OkStatus();
}

absl::Status InstrDecoder::DecodeFenceTypeInstr() {
  VLOG(5) << "Decoding U-type instruction";
  op_ = logic::Opcode::kFenceType;
  pc_sel_ = PcSel::kPcPlus4;
  reg_write_en_ = false;
  mem_op_ = MemOp::kNone;
  return absl::OkStatus();
}

absl::Status InstrDecoder::DecodeETypeInstr() {
  VLOG(5) << "Decoding E-type instruction";
  op_ = logic::Opcode::kEType;
  pc_sel_ = PcSel::kPcPlus4;
  reg_write_en_ = false;
  mem_op_ = MemOp::kNone;

  ASSIGN_OR_RETURN(const uint32_t sel, instr_.GetRs2());
  LOG(INFO) << "FUNC7: " << sel;
  switch (sel) {
   case 0b1:  // ebreak
    e_sel_ = ESel::kEBreak;
    break;
   case 0b0:
    e_sel_ = ESel::kECall;
    break;
   default:
    return absl::InternalError("invalid system call");
  }
  return absl::OkStatus();
}

absl::Status InstrDecoder::Decode(const logic::Wire instr) {
  instr_ = instr;
  absl::StatusOr<const logic::Opcode> opcode = instr.GetOpcode();
  if (absl::IsNotFound(opcode.status())) {
    return absl::InvalidArgumentError("illegal instruction found");
  }

  switch (opcode) {
   case logic::Opcode::kRType:
    return DecodeRTypeInstr();
   case logic::Opcode::kIType:
    return DecodeITypeInstr();
   case logic::Opcode::kLType:
    return DecodeILTypeInstr();
   case logic::Opcode::kSType:
    return DecodeSTypeInstr();
   case logic::Opcode::kBType:
    return DecodeBTypeInstr();
   case logic::Opcode::kLuiType:
    return DecodeLuiTypeInstr();
   case logic::Opcode::kAuiPcType:
    return DecodeAuiPcTypeInstr();
   case logic::Opcode::kJalType:
    return DecodeJalTypeInstr();
   case logic::Opcode::kJalrType:
    return DecodeJalrTypeInstr();
   case logic::Opcode::kFenceType:
    return DecodeFenceTypeInstr();
   case logic::Opcode::kEType:
    return DecodeETypeInstr();
   default:
    // This case should be caught by the above check.
    return absl::InternalError("Instruction decoder encountered unsupported opcode");
  }
}

}  // namespace riscv_emu::decoder
