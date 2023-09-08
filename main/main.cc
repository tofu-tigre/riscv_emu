#include <iostream>
#include <string>

#include "lib/logic/wire.h"
#include "absl/status/statusor.h"

int main(int argc, char* argv[]) {
  riscv_emu::logic::Wire wire;
  wire.u32 = 0b0110011;

  absl::StatusOr<riscv_emu::logic::Opcode> opcode = riscv_emu::logic::GetOpcode(wire);
  if (!opcode.status().ok()) {
    std::cout << opcode.status() << std::endl;
  } else {
    std::cout << "Found valid opcode!\n";
  }

  return 0;
}