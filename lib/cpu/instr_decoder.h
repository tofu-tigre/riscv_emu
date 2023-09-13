#ifndef LIB_CPU_INSTR_DECODER_H
#define LIB_CPU_INSTR_DECODER_H

#include "lib/logic/wire.h"
#include "lib/immediates/imm_decoder.h"
#include "lib/alu/alu.h"
#include "lib/memory/dram.h"
#include "lib/branch_cmp/branch_cmp.h"
#include "absl/status/status.h"

namespace riscv_emu::decoder {

enum class PcSel {
    kPcPlus4,
    kAluOut,
    // No `kNone` field since pc select
    // should ALWAYS be specified.
};

enum class WbSel {
    kMemOut,
    kAluOut,
    kPcPlus4,
    kNone,
};

enum class ASel {
    kPcOut,
    kRegOut,
    kNone,
};

enum class BSel {
    kImmOut,
    kRegOut,
    kNone,
};

enum class MemOp {
    kRead,
    kWrite,
    kNone,
};

enum class ESel {
    kEBreak,
    kECall,
    kNone,
};

class InstrDecoder final {
 public:
  absl::Status Decode(logic::Wire instr);
  absl::Status SetBranchComp(branch::ComparisonResult result);
  inline ASel GetASel() const { return a_sel_; }
  inline BSel GetBSel() const { return b_sel_; }
  inline PcSel GetPcSel() const { return pc_sel_; }
  inline AluOp GetAluSel() const { return alu_sel_; }
  inline logic::Wire GetRs1() const { return rs1_sel_; }
  inline logic::Wire GetRs2() const { return rs2_sel_; }
  inline logic::Wire GetRd() const { return rd_sel_; }
  inline bool GetRegWriteEn() const { return reg_write_en_; }
  inline imm::ImmSel GetImmSel() const { return imm_sel_; }
  inline memory::AccessType GetMemSel() const { return mem_sel_; }
  inline WbSel GetWbSel() const { return wb_sel_; }
  inline MemOp GetMemOp() const { return mem_op_; }
  inline bool IsBranchUnsigned() const { return is_branch_unsigned_; }
  inline ESel GetESel() const { return e_sel_; }
  

 private:
  absl::Status DecodeRTypeInstr(logic::Wire instr);
  absl::Status DecodeITypeInstr(logic::Wire instr);
  absl::Status DecodeILTypeInstr(logic::Wire instr);
  absl::Status DecodeSTypeInstr(const logic::Wire instr);
  absl::Status DecodeBTypeInstr(const logic::Wire instr);
  absl::Status DecodeLuiTypeInstr(const logic::Wire instr);
  absl::Status DecodeAuiPcTypeInstr(const logic::Wire instr);
  absl::Status DecodeJalTypeInstr(const logic::Wire instr);
  absl::Status DecodeJalrTypeInstr(const logic::Wire instr);
  absl::Status DecodeFenceTypeInstr(const logic::Wire instr);
  absl::Status DecodeETypeInstr(const logic::Wire instr);

  bool is_invalid_instr_ = false;
  logic::Wire instr_;
  logic::Wire rs1_sel_;
  logic::Wire rs2_sel_;
  logic::Wire rd_sel_;
  logic::Opcode op_;
  PcSel pc_sel_ = PcSel::kPcPlus4;
  imm::ImmSel imm_sel_  = imm::ImmSel::kNone;
  bool reg_write_en_ = false;
  MemOp mem_op_ = MemOp::kNone;
  memory::AccessType mem_sel_ = memory::AccessType::kNone;
  ASel a_sel_ = ASel::kNone;
  BSel b_sel_ = BSel::kNone;
  AluOp alu_sel_ = AluOp::kNone;
  WbSel wb_sel_ = WbSel::kNone;
  bool is_branch_unsigned_;
  ESel e_sel_ = ESel::kNone;

};

}  // namespace riscv_emu::decoder

#endif  // LIB_CPU_INSTR_DECODER_H
