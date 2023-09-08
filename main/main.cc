#include <iostream>
#include <string>

#include "lib/logic/wire.h"
#include "absl/status/statusor.h"
#include "absl/status/status.h"
#include "glog/logging.h"
#include "lib/alu/alu.h"
#include "lib/immediates/imm_decoder.h"


int main(int argc, char* argv[]) {
  // This may occasionally cause a deadlock with glibc
  // without libunwind on failure.
  google::InstallFailureSignalHandler();
  FLAGS_logtostderr = 1;
  google::InitGoogleLogging(argv[0]);
  
  return 0;
}