#include "wire.h"

namespace riscv_emu::logic {

  bool IsLastBitSet(Wire val) {
    if (val.u32 & (1 << 31)) {
      return true;
    }
    return false;
  }

}
