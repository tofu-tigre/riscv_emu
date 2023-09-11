#ifndef LIB_MEMORY_DRAM_H
#define LIB_MEMORY_DRAM_H

#include <stdint.h>
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "lib/logic/wire.h"

namespace riscv_emu::memory {
  
  namespace constants {
    // Note that `kDramSize` must be word-aligned to prevent logic
    // from accessing non-existing memory (out of range).
    constexpr size_t kDramSize = 1024 * 100;  // 100 KiB
    constexpr uint32_t kDramFetchMask = 0b11111;

  }  // namespace constants

enum class AccessType {
  kByte = 0b000,
  kHalfword = 0b001,
  kWord = 0b010,
  kByteUnsigned = 0b100,
  kHalfwordUnsigned = 0b101,
};

class Dram {
 public:
  // TODO: Consider including `AccessType` as an parameter?
  absl::StatusOr<logic::Wire> Read(size_t at_index);
  absl::Status Write(size_t at_index, logic::Wire val);
  absl::Status Flash(absl::string_view filename);
  inline void SetAccessType(const AccessType type) { access_type_ = type; }

 private:
  uint8_t data_[constants::kDramSize] {0};
  AccessType access_type_;
};

}  // namespace riscv_emu::memory

#endif  // LIB_MEMORY_DRAM_H
