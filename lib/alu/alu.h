#ifndef LIB_ALU_ALU_H
#define LIB_ALU_ALU_H

#include "lib/logic/wire.h"
#include "absl/status/statusor.h"

namespace riscv_emu {

  namespace alu::constants {

    constexpr uint32_t kMaxShiftMask = 0b11111;

  }  // namespace alu::constants

enum class AluOp {
  kAdd,
  kSub,
  kOr,
  kAnd,
  kXor,
  kSll,  // Shift left logical
  kSra,  // Shift right arithmetic
  kSrl,  // Shift right logical
};

class Alu final {
 private:
  bool hasOverflow_;

 public:
  Alu() : hasOverflow_(false) {}
  absl::StatusOr<logic::Wire> DoOp(AluOp op, logic::Wire val1, logic::Wire val2);


};

}  // namespace riscv_emu

#endif  // LIB_ALU_ALU_H