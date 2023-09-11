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

    logic::Wire DecodeUTypeImm(const logic::Wire input) {
      uint32_t output = input.GetUnsigned() & constants::kUTypeImmMask;
      return logic::Wire(output);
    }

    logic::Wire DecodeJTypeImm(const logic::Wire input) {
      const uint32_t valUpper = (input.GetUnsigned() & constants::kJTypeUpperImmMask) >> constants::kJTypeUpperImmShift;
      const uint32_t valUpperMiddle = (input.GetUnsigned() & constants::kJTypeUpperMiddleImmMask) >> constants::kJTypeUpperMiddleImmShift;
      const uint32_t valLowerMiddle = (input.GetUnsigned() & constants::kJTypeLowerMiddleImmMask) >> constants::kJTypeLowerMiddleImmShift;
      // J-type immediates have lowest bit == 0.
      const uint32_t valLower = (input.GetUnsigned() & constants::kJTypeLowerImmMask) >> (constants::kJTypeLowerImmShift - 1);
      uint32_t output = (valUpper << constants::kJTypeUpperImmCombinedShift) |
                     (valUpperMiddle << constants::kJTypeUpperMiddleImmCombinedShift) |
                     (valLowerMiddle << constants::kJTypeLowerMiddleImmCombinedShift) |
                     valLower;
      if (output & (1 << constants::kJTypeImmSignedBit)) {
        output |= constants::kJTypeImmSignExtMask;
      }
      return logic::Wire(output);
    }

  }  // namespace

absl::StatusOr<logic::Wire> DecodeImm(const ImmSel imm_sel, const logic::Wire input) {
  switch(imm_sel) {
   case ImmSel::kIType:
    return DecodeITypeImm(input);
   case ImmSel::kSType:
    return DecodeSTypeImm(input);
   case ImmSel::kBType:
    return DecodeBTypeImm(input);
   case ImmSel::kUType:
    return DecodeUTypeImm(input);
   case ImmSel::kJType:
    return DecodeJTypeImm(input);
   default:
    return absl::InternalError("Immediate decoder select not properly configured");
  }
}

}  // namespace riscv_emu::imm
