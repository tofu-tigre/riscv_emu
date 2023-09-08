#include <iostream>
#include <string>

#include "lib/logic/wire.h"
#include "absl/status/statusor.h"
#include "absl/status/status.h"
#include "glog/logging.h"
#include "lib/alu/alu.h"


int main(int argc, char* argv[]) {
  // This may occasionally cause a deadlock with glibc
  // without libunwind on failure.
  google::InstallFailureSignalHandler();
  FLAGS_logtostderr = 1;
  google::InitGoogleLogging(argv[0]);

  riscv_emu::logic::Wire wire1(0b0000000000000000000000010000001);
  riscv_emu::logic::Wire wire2(0b110000001);
  
  riscv_emu::Alu alu;
  absl::StatusOr<riscv_emu::logic::Wire> output = alu.DoOp(riscv_emu::AluOp::kSra, wire1, wire2);
  LOG(INFO) << *output;
  LOG(INFO) << output->GetSigned();
  return 0;
}