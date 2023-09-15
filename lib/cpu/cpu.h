#ifndef LIB_CPU_CPU_H
#define LIB_CPU_CPU_H

#include <stdint.h>
#include "lib/logic/wire.h"
#include "lib/alu/alu.h"
#include "lib/perfs/bus.h"
#include "instr_decoder.h"
#include "glog/logging.h"
#include "absl/status/status.h"

namespace riscv_emu {

class Cpu final {
 private:
  uint32_t clock_;
  uint32_t pc_ = 0x8000 - 0x4;
  uint32_t instr_; 
  bool power_is_on_;
  Alu alu_;
  perfs::bus::Bus bus_;
  decoder::InstrDecoder decoder_;

  uint32_t alu_out_;
  uint32_t mem_out_;
  uint32_t a_out_;
  uint32_t b_out_;

  uint32_t registers_[32] { 0 };

  absl::Status Fetch();
  absl::Status Decode();
  absl::Status Execute();
  absl::Status Memory();
  absl::Status Writeback();

 public:
  Cpu() = default;
  absl::Status Boot();
};

}  // namespace riscv_emu

#endif  // LIB_CPU_CPU_H
