#ifndef LIB_LOGIC_OPCODES_H
#define LIB_LOGIC_OPCODES_H

#include <stdint.h>

namespace riscv_emu::logic {

namespace constants {

  constexpr uint32_t kOpcodeMask = 0b1111111;

}  // namespace constants

enum class Opcode : uint32_t {
  R_TYPE = 0b0110011,
  I_TYPE = 0b0010011,
  S_TYPE = 0b0100011,
  B_TYPE = 0b1100011,
  U_TYPE = 0b0000011,
  J_TYPE = 0b1101111,
};

}

#endif
