#include <iostream>
#include <string>

#include "absl/status/statusor.h"
#include "absl/status/status.h"
#include "glog/logging.h"
#include "lib/cpu/cpu.h"


int main(int argc, char* argv[]) {
  // This may occasionally cause a deadlock with glibc
  // without libunwind on failure.
  google::InstallFailureSignalHandler();
  FLAGS_logtostderr = 1;
  google::InitGoogleLogging(argv[0]);

  riscv_emu::Cpu cpu;
  cpu.Boot();
  
  return 0;
}