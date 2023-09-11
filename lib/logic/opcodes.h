#ifndef LIB_LOGIC_OPCODES_H
#define LIB_LOGIC_OPCODES_H

#include <stdint.h>

namespace riscv_emu::logic {

namespace constants {

  constexpr uint32_t kOpcodeMask = 0b1111111;

}  // namespace constants

enum class Opcode : uint32_t {
  kRType = 0b0110011,
  kIType = 0b0010011,
  kSType = 0b0100011,
  kBType = 0b1100011,
  kLuiType = 0b0110111,  // lui
  kAuiPcType = 0b0010111,  // auipc
  kEType = 0b1110011,  // ebreak, ecall
  kFenceType = 0b0001111,
  kJType = 0b1101111,
  kLType = 0b0000011,  // lb, lh, lw
};

}  // namespace riscv_emu::logic

#endif  // LIB_LOGIC_OPCODES_H
