#ifndef LIB_CPU_CPU_H
#define LIB_CPU_CPU_H

#include <stdint.h>
#include "lib/logic/wire.h"
#include "lib/alu/alu.h"
#include "absl/status/status.h"

namespace riscv_emu {

class Cpu final {
 private:
  logic::Wire pc_;
  logic::Wire instr_; 
  bool power_is_on_;
  Alu alu_;

  logic::Wire registers_[32];

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
