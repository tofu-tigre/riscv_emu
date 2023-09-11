#ifndef LIB_CPU_CPU_H
#define LIB_CPU_CPU_H

#include <stdint.h>
#include "lib/logic/register.h"
#include "lib/logic/wire.h"
#include "lib/alu/alu.h"
#include "lib/memory/dram.h"
#include "instr_decoder.h"
#include "glog/logging.h"
#include "absl/status/status.h"

namespace riscv_emu {

class Cpu final {
 private:
  logic::Register clock_;
  logic::Register pc_;
  logic::Register instr_; 
  bool power_is_on_;
  Alu alu_;
  memory::Dram dmem_;
  memory::Dram imem_;
  InstrDecoder decoder_;

  logic::Wire alu_out_;
  logic::Wire mem_out_;
  logic::Wire a_out_;
  logic::Wire b_out_;

  logic::Register registers_[32];

  absl::Status Fetch();
  absl::Status Decode();
  absl::Status Execute();
  absl::Status Memory();
  absl::Status Writeback();

 public:
  Cpu();
  absl::Status Boot();
};

}  // namespace riscv_emu

#endif  // LIB_CPU_CPU_H
