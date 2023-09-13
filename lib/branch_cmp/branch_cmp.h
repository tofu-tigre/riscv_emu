#ifndef LIB_BRANCH_CMP_BRANCH_CMP_H
#define LIB_BRANCH_CMP_BRANCH_CMP_H

#include <cstdint>
#include "lib/logic/wire.h"

namespace riscv_emu::branch {

enum class ComparisonType {
  kEqual =  0b000,
  kNotEqual =  0b001,
  kLessThan =  0b100,
  kGreaterThanOrEqual =  0b101,
  kLessThanUnsigned = 0b110,
  kGreaterThanOrEqualUnsigned = 0b111,
};

struct ComparisonResult {
    bool branch_eq_;
    bool branch_lt_;
};

ComparisonResult DoBranchComp(bool is_unsigned, uint32_t val1, uint32_t val2);

}  // namespace riscv_emu::branch

#endif  // LIB_BRANCH_CMP_BRANCH_CMP_H
