#ifndef LIB_IMMEDIATES_IMM_DECODER_H
#define LIB_IMMEDIATES_IMM_DECODER_H

#include "absl/status/statusor.h"
#include "lib/logic/opcodes.h"
#include "lib/logic/wire.h"

namespace riscv_emu::imm {

  namespace constants {

    constexpr int kITypeImmShift = 20;
    constexpr uint32_t kITypeImmMask = 0b111111111111 << kITypeImmShift;
    constexpr int kITypeImmSignedBit = 11;
    constexpr uint32_t kITypeImmSignExtMask = 0b11111111111111111111 << (kITypeImmSignedBit + 1);

    constexpr int kSTypeLowerImmShift = 7;
    constexpr uint32_t kSTypeLowerImmMask = 0b11111 << kSTypeLowerImmShift;
    constexpr int kSTypeUpperImmShift = 25;
    constexpr uint32_t kSTypeUpperImmMask = 0b1111111 << kSTypeUpperImmShift;
    constexpr int kSTypeUpperImmCombinedShift = 5;
    constexpr int kSTypeImmSignedBit = 11;
    constexpr uint32_t kSTypeImmSignExtMask = 0b11111111111111111111 << (kSTypeImmSignedBit + 1);

    constexpr int kBTypeLowerImmShift = 8;
    constexpr uint32_t kBTypeLowerImmMask = 0b1111 << kBTypeLowerImmShift;
    constexpr int kBTypeLowerMiddleImmShift = 25;
    constexpr uint32_t kBTypeLowerMiddleImmMask = 0b111111 << kBTypeLowerMiddleImmShift;
    constexpr int kBTypeLowerMiddleImmCombinedShift = 5;
    constexpr int kBTypeUpperMiddleImmShift = 7;
    constexpr uint32_t kBTypeUpperMiddleImmMask = 0b1 << kBTypeUpperMiddleImmShift;
    constexpr int kBTypeUpperMiddleImmCombinedShift = 11;
    constexpr int kBTypeUpperImmShift = 31;
    constexpr uint32_t kBTypeUpperImmMask = 0b1 << kBTypeUpperImmShift;
    constexpr int kBTypeUpperImmCombinedShift = 12;
    constexpr int kBTypeImmSignedBit = 12;
    constexpr uint32_t kBTypeImmSignExtMask = 0b1111111111111111111 << (kBTypeImmSignedBit + 1);

    constexpr int kUTypeImmShift = 12;
    constexpr uint32_t kUTypeImmMask = 0b11111111111111111111 << kUTypeImmShift;

    constexpr int kJTypeLowerImmShift = 21;
    constexpr uint32_t kJTypeLowerImmMask = 0b1111111111 << kJTypeLowerImmShift;
    constexpr int kJTypeLowerMiddleImmShift = 20;
    constexpr uint32_t kJTypeLowerMiddleImmMask = 0b1 << kJTypeLowerMiddleImmShift;
    constexpr int kJTypeLowerMiddleImmCombinedShift = 11;
    constexpr int kJTypeUpperMiddleImmShift = 12;
    constexpr uint32_t kJTypeUpperMiddleImmMask = 0b11111111 << kJTypeUpperMiddleImmShift;
    constexpr int kJTypeUpperMiddleImmCombinedShift = 12;
    constexpr int kJTypeUpperImmShift = 31;
    constexpr uint32_t kJTypeUpperImmMask = 0b1 << kJTypeUpperImmShift;
    constexpr int kJTypeUpperImmCombinedShift = 20;
    constexpr int kJTypeImmSignedBit = 20;
    constexpr uint32_t kJTypeImmSignExtMask = 0b11111111111 << (kJTypeImmSignedBit + 1);

  }  // namespace constants

  enum class ImmSel {
    kSType,
    kIType,
    kBType,
    kUType,
    kJType,
  };

absl::StatusOr<logic::Wire> DecodeImm(ImmSel imm_sel, logic::Wire wire);

}  // namespace riscv_emu::imm

#endif  // LIB_IMMEDIATES_IMM_DECODER_H
