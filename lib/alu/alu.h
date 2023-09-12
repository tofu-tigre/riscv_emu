#ifndef LIB_ALU_ALU_H
#define LIB_ALU_ALU_H

#include "lib/logic/wire.h"
#include "absl/status/statusor.h"

namespace riscv_emu {

  namespace alu::constants {

    constexpr uint32_t kMaxShiftMask = 0b11111;

  }  // namespace alu::constants

enum class AluOp {
  kBCopy = 0b11111,
  kAdd = 0b000,
  kSub = 0b1010, // Same as below, DO NOT USE.
  kOr  = 0b110,
  kAnd = 0b111,
  kXor = 0b100,
  kSll = 0b001,   // Shift left logical
  kSra = 0b101,   // Shift right arithmetic
  kSrl = 0b1111,  // Shift right logical. Note that this is actually the same value as `kSra`
                  // but due to compiler constraints must be a different value. DO NOT USE.
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