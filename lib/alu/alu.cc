#include "alu.h"
#include "status_macros.h"
#include "glog/logging.h"

namespace riscv_emu {

  namespace {

    absl::StatusOr<logic::Wire> Add(const logic::Wire val1, const logic::Wire val2, bool* hasOverflow) {
      const int32_t result = val1.GetSigned() + val2.GetSigned();
      return logic::Wire(result);
    }

    absl::StatusOr<logic::Wire> Sub(const logic::Wire val1, const logic::Wire val2, bool* hasOverflow) {
      const int32_t result = val1.GetSigned() - val2.GetSigned();
      return logic::Wire(result);
    }

    absl::StatusOr<logic::Wire> Or(const logic::Wire val1, const logic::Wire val2, bool* hasOverflow) {
      const int32_t result = val1.GetSigned() | val2.GetSigned();
      return logic::Wire(result);
    }

    absl::StatusOr<logic::Wire> And(const logic::Wire val1, const logic::Wire val2, bool* hasOverflow) {
      const int32_t result = val1.GetSigned() & val2.GetSigned();
      return logic::Wire(result);
    }

    absl::StatusOr<logic::Wire> Xor(const logic::Wire val1, const logic::Wire val2, bool* hasOverflow) {
      const int32_t result = val1.GetSigned() ^ val2.GetSigned();
      return logic::Wire(result);
    }

    absl::StatusOr<logic::Wire> Sll(const logic::Wire val1, const logic::Wire val2, bool* hasOverflow) {
      // Extra protection here: in case `val2` (rs2 & 0b11111) > max shift amount.
      if ((val2.GetUnsigned() & (~alu::constants::kMaxShiftMask)) > 0) {
        return absl::FailedPreconditionError("Instruction attempted to bit shift > 31 bits");
      }
      const uint32_t result = val1.GetUnsigned() << val2.GetUnsigned();
      return logic::Wire(result);
    }

    absl::StatusOr<logic::Wire> Sra(const logic::Wire val1, const logic::Wire val2, bool* hasOverflow) {
      // Extra protection here: in case `val2` (rs2 & 0b11111) > max shift amount.
      if ((val2.GetUnsigned() & (~alu::constants::kMaxShiftMask)) > 0) {
        return absl::FailedPreconditionError("Instruction attempted to bit shift > 31 bits");
      }
      int32_t result = 0;
#ifdef __APPLE__
      asm (
        "mov %1, %%eax\n\t"      // Move first reg. to %eax
        "mov %2, %%ecx\n\t"      // Move second reg. to %%ecx
        "ror %%cl, %%eax\n\t"   // Rotate right
        "mov %%eax, %0"          // Move %%eax to 'result'
        : "=r" (result)
        : "r" (val1.GetSigned()), "r" (val2.GetUnsigned())
        : "eax", "ecx"
    );
#else
      asm (
        "mov %1, %%eax\n\t"      // Move first reg. to %eax
        "mov %2, %%ecx\n\t"      // Move second reg. to %%ecx
        "ror %%ecx, %%eax\n\t"   // Rotate right
        "mov %%eax, %0"          // Move %%eax to 'result'
        : "=r" (result)
        : "r" (val1.GetSigned()), "r" (val2.GetUnsigned())
        : "eax", "ecx"
    );
#endif
      return logic::Wire(result);
    }

    // TODO: Consider moving error checking elsewhere?
    absl::StatusOr<logic::Wire> Srl(const logic::Wire val1, const logic::Wire val2, bool* hasOverflow) {
      // Extra protection here: in case `val2` (rs2 & 0b11111) > max shift amount.
      if ((val2.GetUnsigned() & (~alu::constants::kMaxShiftMask)) > 0) {
        return absl::FailedPreconditionError("Instruction attempted to bit shift > 31 bits");
      }
      const uint32_t result = val1.GetUnsigned() >> val2.GetUnsigned();
      return logic::Wire(result);
    }

  }  // namespace

  absl::StatusOr<logic::Wire> Alu::DoOp(const AluOp op, const logic::Wire val1, const logic::Wire val2) {
    bool hasOverflow = false;
    absl::StatusOr<logic::Wire> output;
    switch (op) {
     case AluOp::kBCopy:
      output = absl::StatusOr<logic::Wire>(val2);
      break;
     case AluOp::kAdd:
      output = Add(val1, val2, &hasOverflow);
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
