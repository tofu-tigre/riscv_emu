#include "branch_cmp.h"

namespace riscv_emu::branch {

namespace {

union Wire {
  uint32_t u32;
  int32_t i32;
  Wire(const uint32_t val) { u32 = val; }
  Wire(const int32_t val) { i32 = val; }
};

}  // namespace

ComparisonResult DoBranchComp(const bool is_unsigned, const uint32_t val1, const uint32_t val2) {
  ComparisonResult result;
  Wire v1 { val1 };
  Wire v2 { val2 };
  
  if (is_unsigned) {
    result.branch_eq_ = v1.u32 == v2.u32;
    result.branch_lt_ = v1.u32 < v2.u32;
  } else {
    result.branch_eq_ = v1.i32 == v2.i32;
    result.branch_lt_ = v1.i32 < v2.i32;
  }
  return result;
}

}  // namespace riscv_emu::branch