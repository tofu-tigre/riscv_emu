#ifndef LIB_CPU_CPU_H
#define LIB_CPU_CPU_H

#include <stdint.h>
#include "lib/logic/register.h"
#include "lib/alu/alu.h"
#include "absl/status/status.h"

namespace riscv_emu {

class Cpu final {
 private:
  logic::Register clock_;
  logic::Register pc_;
  logic::Register instr_; 
  bool power_is_on_;
  Alu alu_;

  logic::Register registers_[32];

  absl::Status Fetch();
  absl::Status Decode();
  absl::Status Execute();
  absl::Status Memory();
  absl::Status Writeback();

 public:
  absl::Status Boot();
};

}  // namespace riscv_emu

#endif  // LIB_CPU_CPU_H
