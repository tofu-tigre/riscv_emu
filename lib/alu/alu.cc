#include "alu.h"
#include "status_macros.h"
#include "glog/logging.h"

namespace riscv_emu {

namespace {

union Wire {
  uint32_t u32;
  int32_t i32;
  Wire(const uint32_t val) { u32 = val; }
  Wire(const int32_t val) { i32 = val; }
};

absl::StatusOr<uint32_t> Add(const Wire val1, const Wire val2, bool* hasOverflow) {
  const Wire result { val1.i32 + val2.i32 };
  return result.u32;
}

absl::StatusOr<uint32_t> Sub(const Wire val1, const Wire val2, bool* hasOverflow) {
  const Wire result { val1.i32 - val2.i32 };
  return result.u32;
}

absl::StatusOr<uint32_t> Or(const Wire val1, const Wire val2, bool* hasOverflow) {
  const Wire result { val1.i32 | val2.i32 };
  return result.u32;
}

absl::StatusOr<uint32_t> And(const Wire val1, const Wire val2, bool* hasOverflow) {
  const Wire result { val1.i32 & val2.i32 };
  return result.u32;
}

absl::StatusOr<uint32_t> Xor(const Wire val1, const Wire val2, bool* hasOverflow) {
  const Wire result { val1.i32 ^ val2.i32 };
  return result.u32;
}

absl::StatusOr<uint32_t> Sll(const Wire val1, const Wire val2, bool* hasOverflow) {
  const Wire result = val1.u32 << (val2.u32 & alu::constants::kMaxShiftMask);
  return result.u32;
}

absl::StatusOr<uint32_t> Sra(const Wire val1, const Wire val2, bool* hasOverflow) {
  Wire result { 0 };
#ifdef __APPLE__
  asm (
    "mov %1, %%eax\n\t"      // Move first reg. to %eax
    "mov %2, %%ecx\n\t"      // Move second reg. to %%ecx
    "ror %%cl, %%eax\n\t"   // Rotate right
    "mov %%eax, %0"          // Move %%eax to 'result'
    : "=r" (result.i32)
    : "r" (val1.u32), "r" (val2.u32 & alu::constants::kMaxShiftMask)
    : "eax", "ecx"
  );
#else
  asm (
    "mov %1, %%eax\n\t"      // Move first reg. to %eax
    "mov %2, %%ecx\n\t"      // Move second reg. to %%ecx
    "ror %%ecx, %%eax\n\t"   // Rotate right
    "mov %%eax, %0"          // Move %%eax to 'result'
    : "=r" (result.u32)
    : "r" (val1.u32), "r" (val2.u32 & alu::constants::kMaxShiftMask)
    : "eax", "ecx"
  );
#endif
  return result.u32;
}

absl::StatusOr<uint32_t> Srl(const Wire val1, const Wire val2, bool* hasOverflow) {
  const Wire result = val1.u32 >> (val2.u32 & alu::constants::kMaxShiftMask);
  return result.u32;
}

}  // namespace

absl::StatusOr<uint32_t> Alu::DoOp(const AluOp op, const uint32_t val1, const uint32_t val2) {
  bool hasOverflow = false;
  absl::StatusOr<uint32_t> output;
  switch (op) {
    case AluOp::kBCopy:
    output = absl::StatusOr<uint32_t>(val2);
    break;
    case AluOp::kAdd:
    output = Add(val1, val2, &hasOverflow);
    break;
    case AluOp::kAddAddr:
    output = Add(val1, val2 ^ 0b1, &hasOverflow);
    break;
    case AluOp::kSub:
    output = Sub(val1, val2, &hasOverflow);
    break;
    case AluOp::kAnd:
    output = And(val1, val2, &hasOverflow);
    break;
    case AluOp::kOr:
    output = Or(val1, val2, &hasOverflow);
    break;
    case AluOp::kXor:
    output = Xor(val1, val2, &hasOverflow);
    break;
    case AluOp::kSll:
    output = Sll(val1, val2, &hasOverflow);
    break;
    case AluOp::kSra:
    output =  Sra(val1, val2, &hasOverflow);
    break;
    case AluOp::kSrl:
    output =  Srl(val1, val2, &hasOverflow);
    break;
    case AluOp::kNone:
    break;
    default:
    return absl::InternalError("Invalid ALU operation");
  }
  if (!output.ok()) {
    return output;
  }
  hasOverflow_ = hasOverflow;
  return *output;
}

}  // namespace riscv_emu
