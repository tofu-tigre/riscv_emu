#include "imm_decoder.h"
#include "status_macros.h"
#include "glog/logging.h"

namespace riscv_emu::imm {

  namespace {

    logic::Wire DecodeITypeImm(const logic::Wire input) {
      uint32_t output = (input.GetUnsigned() & constants::kITypeImmMask) >> constants::kITypeImmShift;
      if (output & (1 << constants::kITypeImmSignedBit)) {
        output |= constants::kITypeImmSignExtMask;
      }
      return logic::Wire(output);
    }

    logic::Wire DecodeSTypeImm(const logic::Wire input) {
      const uint32_t valUpper = (input.GetUnsigned() & constants::kSTypeUpperImmMask) >> constants::kSTypeUpperImmShift;
      const uint32_t valLower = (input.GetUnsigned() & constants::kSTypeLowerImmMask) >> constants::kSTypeLowerImmShift;
      uint32_t output = (valUpper << constants::kSTypeUpperImmCombinedShift) | valLower;
      if (output & (1 << constants::kSTypeImmSignedBit)) {
        output |= constants::kSTypeImmSignExtMask;
      }
      return logic::Wire(output);
    }

    logic::Wire DecodeBTypeImm(const logic::Wire input) {
      const uint32_t valUpper = (input.GetUnsigned() & constants::kBTypeUpperImmMask) >> constants::kBTypeUpperImmShift;
      const uint32_t valUpperMiddle = (input.GetUnsigned() & constants::kBTypeUpperMiddleImmMask) >> constants::kBTypeUpperMiddleImmShift;
      const uint32_t valLowerMiddle = (input.GetUnsigned() & constants::kBTypeLowerMiddleImmMask) >> constants::kBTypeLowerMiddleImmShift;
      // B-type immediates have lowest bit == 0.
      const uint32_t valLower = (input.GetUnsigned() & constants::kBTypeLowerImmMask) >> (constants::kBTypeLowerImmShift - 1);
      uint32_t output = (valUpper << constants::kBTypeUpperImmCombinedShift) |
                     (valUpperMiddle << constants::kBTypeUpperMiddleImmCombinedShift) |
                     (valLowerMiddle << constants::kBTypeLowerMiddleImmCombinedShift) |
                     valLower;
      if (output & (1 << constants::kBTypeImmSignedBit)) {
        output |= constants::kBTypeImmSignExtMask;
      }
      return logic::Wire(output);
    }

  }  // namespace

absl::StatusOr<logic::Wire> DecodeImm(const logic::Wire input) {
  ASSIGN_OR_RETURN(logic::Opcode opcode, input.GetOpcode());
  switch(opcode) {
   case logic::Opcode::I_TYPE:
    return DecodeITypeImm(input);
   case logic::Opcode::S_TYPE:
    return DecodeSTypeImm(input);
   case logic::Opcode::B_TYPE:
    return DecodeBTypeImm(input);
   case logic::Opcode::U_TYPE:
   case logic::Opcode::J_TYPE:

   case logic::Opcode::R_TYPE:
    return absl::InvalidArgumentError("R-type instructions do not support immediates");
   default:
    return absl::InvalidArgumentError("Invalid opcode");
  }
}

}  // namespace riscv_emu::imm
