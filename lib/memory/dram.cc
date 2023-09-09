#include "dram.h"
#include "status_macros.h"

namespace riscv_emu::memory {

  namespace {

    absl::StatusOr<bool> IsUnalignedAccess(const AccessType access_type, size_t at_index) {
      switch (access_type) {
       case AccessType::kByte:
        return false;
       case AccessType::kHalfword:
        if (at_index  % sizeof(uint16_t) != 0) {
          return true;
        }
       case AccessType::kWord:
       if (at_index  % sizeof(uint32_t) != 0) {
          return true;
        }
       default:
        return absl::InternalError("`IsUnalignedAccess()` reached impossible case");
      }
      return false;
    }

  }  // namespace

absl::StatusOr<logic::Wire> Dram::Read(const size_t at_index) {
  if (at_index >= constants::kDramSize) {
    return absl::OutOfRangeError("Memory address out of range");
  }
  ASSIGN_OR_RETURN(const bool isUnaligned, IsUnalignedAccess(access_type_, at_index));
  if (isUnaligned) {
    return absl::FailedPreconditionError("unaligned memory access detected");
  }
  uint32_t* data_32 = reinterpret_cast<uint32_t*>(data_32);
  const size_t at_rounded_index = at_index & (~constants::kDramFetchMask);

  return logic::Wire(data_32[at_rounded_index]);
}

absl::Status Write(const size_t at_index, const logic::Wire val) {

}

}  // namespace riscv_emu::memory
