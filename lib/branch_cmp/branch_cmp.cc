#include "branch_cmp.h"

namespace riscv_emu::branch {

ComparisonResult DoBranchComp(const bool is_unsigned, const logic::Wire val1, const logic::Wire val2) {
  ComparisonResult result;
  if (is_unsigned) {
    result.branch_eq_ = val1.GetUnsigned() == val2.GetUnsigned();
    result.branch_lt_ = val1.GetUnsigned() < val2.GetUnsigned();
  } else {
    result.branch_eq_ = val1.GetSigned() == val2.GetSigned();
    result.branch_lt_ = val1.GetSigned() < val2.GetSigned();
  }
  return result;
}

}  // namespace riscv_emu::branch