#ifndef LIB_MEMORY_DRAM_H
#define LIB_MEMORY_DRAM_H

#include <stdint.h>
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "lib/logic/wire.h"

namespace riscv_emu::memory {
  
  namespace constants {

    constexpr size_t kDramSize = 1024 * 100;  // 100 KiB
    constexpr uint32_t kDramFetchMask = 0b11111;

  }  // namespace constants

enum class AccessType {
  kByte,
  kHalfword,
  kWord,
};

class Dram final {
 public:
  absl::StatusOr<logic::Wire> Read(size_t at_index);
  absl::Status Write(size_t at_index, logic::Wire val);

 private:
  uint8_t data_[constants::kDramSize];
  AccessType access_type_;

};

}  // namespace riscv_emu::memory

#endif  // LIB_MEMORY_DRAM_H
