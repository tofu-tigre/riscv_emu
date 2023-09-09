#include "cpu.h"
#include "status_macros.h"
#include "absl/status/statusor.h"
#include "lib/logic/wire.h"

namespace riscv_emu {

  namespace {

    absl::StatusOr<logic::Wire> GetRegister(logic::Register registers[32], const size_t at_index) {
      if (at_index > 31) {
        return absl::OutOfRangeError("Cannot access register at index > 31");
      }
      return registers[at_index].GetOut();
    }

    absl::Status SetRegister(logic::Register registers[32], const size_t at_index, const logic::Wire val) {
      if (at_index > 31) {
        return absl::OutOfRangeError("Cannot access register at index > 31");
      }
      registers[at_index].SetIn(val);
    }

  }  // namespace

absl::Status Cpu::Fetch() {
  return absl::OkStatus();
}

absl::Status Cpu::Decode() {
  return absl::OkStatus();
}

absl::Status Cpu::Execute() {
  return absl::OkStatus();
}

absl::Status Cpu::Memory() {
  return absl::OkStatus();
}

absl::Status Cpu::Writeback() {
  return absl::OkStatus();
}

absl::Status Cpu::Boot() {
  power_is_on_ = true;

  while (power_is_on_) {
    RETURN_IF_ERROR(Fetch());
    RETURN_IF_ERROR(Decode());
    RETURN_IF_ERROR(Execute());
    RETURN_IF_ERROR(Memory());
    RETURN_IF_ERROR(Writeback());
  }

  return absl::OkStatus();
}

}  // namespace riscv_emu
