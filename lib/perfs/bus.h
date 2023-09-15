#ifndef LIB_PERFS_BUS_H
#define LIB_PERFS_BUS_H

#include <cstdint>
#include "lib/memory/dram.h"
// #include "uart.h"
#include "glog/logging.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"

namespace riscv_emu::perfs::bus {

namespace constants {
  constexpr uint32_t kDramStartAddr = 0x0;
  constexpr uint32_t kDramEndAddr = kDramStartAddr + memory::constants::kDramSize;  // 0x000fa000
  constexpr uint32_t kUartStartAddr = 0x0fff0000;
}  // namespace constants

class Bus final {
 public:
  Bus() { dram_.Flash("/tmp/progs/foo.o").IgnoreError(); }
  absl::Status Write(uint32_t addr, uint32_t val);
  absl::StatusOr<uint32_t> Read(uint32_t addr);
  inline void SetDramAccessType(memory::AccessType access_type) { dram_.SetAccessType(access_type); }
  

 private:
  memory::Dram dram_;
};

}  // namespace riscv_emu::perfs::bus

#endif  // LIB_PERFS_BUS_H