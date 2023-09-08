#ifndef LIB_LOGIC_WIRE_H
#define LIB_LOGIC_WIRE_H

#include <stdint.h>
#include "lib/logic/opcodes.h"
#include "absl/status/statusor.h"

namespace riscv_emu::logic {

  union Wire {
    uint32_t u32;
    int32_t i32;
  };

  absl::StatusOr<Opcode> GetOpcode(Wire wire) {
    const uint32_t opcode = wire.u32 & constants::kOpcodeMask;
    switch (opcode) {
      case static_cast<uint32_t>(Opcode::R_TYPE):
        return Opcode::R_TYPE;
        break;
      case static_cast<uint32_t>(Opcode::I_TYPE):
        return Opcode::I_TYPE;
        break;
      case static_cast<uint32_t>(Opcode::S_TYPE):
        return Opcode::S_TYPE;
        break;
      case static_cast<uint32_t>(Opcode::B_TYPE):
        return Opcode::B_TYPE;
        break;
      case static_cast<uint32_t>(Opcode::U_TYPE):
        return Opcode::U_TYPE;
        break;
      case static_cast<uint32_t>(Opcode::J_TYPE):
        return Opcode::J_TYPE;
        break;
      default:
        return absl::InvalidArgumentError("No opcode found");
    }
  }

}

#endif