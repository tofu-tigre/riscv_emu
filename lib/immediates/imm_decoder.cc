#include "imm_decoder.h"
#include "status_macros.h"
#include "glog/logging.h"

namespace riscv_emu::imm {

namespace {

uint32_t DecodeITypeImm(const uint32_t input) {
  uint32_t output = (input & constants::kITypeImmMask) >> constants::kITypeImmShift;
  if (output & (1 << constants::kITypeImmSignedBit)) {
    output |= constants::kITypeImmSignExtMask;
  }
  return output;
}

uint32_t DecodeSTypeImm(const uint32_t input) {
  const uint32_t valUpper = (input & constants::kSTypeUpperImmMask) >> constants::kSTypeUpperImmShift;
  const uint32_t valLower = (input & constants::kSTypeLowerImmMask) >> constants::kSTypeLowerImmShift;
  uint32_t output = (valUpper << constants::kSTypeUpperImmCombinedShift) | valLower;
  if (output & (1 << constants::kSTypeImmSignedBit)) {
    output |= constants::kSTypeImmSignExtMask;
  }
  return output;
}

uint32_t DecodeBTypeImm(const uint32_t input) {
  const uint32_t valUpper = (input & constants::kBTypeUpperImmMask) >> constants::kBTypeUpperImmShift;
  const uint32_t valUpperMiddle = (input & constants::kBTypeUpperMiddleImmMask) >> constants::kBTypeUpperMiddleImmShift;
  const uint32_t valLowerMiddle = (input & constants::kBTypeLowerMiddleImmMask) >> constants::kBTypeLowerMiddleImmShift;
  // B-type immediates have lowest bit == 0.
  const uint32_t valLower = (input & constants::kBTypeLowerImmMask) >> (constants::kBTypeLowerImmShift - 1);
  uint32_t output = (valUpper << constants::kBTypeUpperImmCombinedShift) |
                  (valUpperMiddle << constants::kBTypeUpperMiddleImmCombinedShift) |
                  (valLowerMiddle << constants::kBTypeLowerMiddleImmCombinedShift) |
                  valLower;
  if (output & (1 << constants::kBTypeImmSignedBit)) {
    output |= constants::kBTypeImmSignExtMask;
  }
  return output;
}

uint32_t DecodeUTypeImm(const uint32_t input) {
  uint32_t output = input & constants::kUTypeImmMask;
  return output;
}

uint32_t DecodeJTypeImm(const uint32_t input) {
  const uint32_t valUpper = (input & constants::kJTypeUpperImmMask) >> constants::kJTypeUpperImmShift;
  const uint32_t valUpperMiddle = (input & constants::kJTypeUpperMiddleImmMask) >> constants::kJTypeUpperMiddleImmShift;
  const uint32_t valLowerMiddle = (input & constants::kJTypeLowerMiddleImmMask) >> constants::kJTypeLowerMiddleImmShift;
  // J-type immediates have lowest bit == 0.
  const uint32_t valLower = (input & constants::kJTypeLowerImmMask) >> (constants::kJTypeLowerImmShift - 1);
  uint32_t output = (valUpper << constants::kJTypeUpperImmCombinedShift) |
                  (valUpperMiddle << constants::kJTypeUpperMiddleImmCombinedShift) |
                  (valLowerMiddle << constants::kJTypeLowerMiddleImmCombinedShift) |
                  valLower;
  if (output & (1 << constants::kJTypeImmSignedBit)) {
    output |= constants::kJTypeImmSignExtMask;
  }
  return output;
}

}  // namespace

absl::StatusOr<uint32_t> DecodeImm(const ImmSel imm_sel, const uint32_t input) {
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
