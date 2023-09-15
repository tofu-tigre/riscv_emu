#include "bus.h"
#include <iostream>
#include "status_macros.h"

namespace riscv_emu::perfs::bus {

absl::Status Bus::Write(uint32_t addr, uint32_t val) {
  if (addr == constants::kUartStartAddr) {
    // Ignore upper 3 bytes.
    std::cout << *(reinterpret_cast<char*>(&val));
    return absl::OkStatus();
  }
  return dram_.Write(addr, val);
}

absl::StatusOr<uint32_t> Bus::Read(uint32_t addr) {
  // Redirect address to proper device.
  return dram_.Read(addr);
}

}  // namespace riscv_emu::perfs::bus