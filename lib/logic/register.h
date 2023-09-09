#ifndef LIB_LOGIC_REGISTER_H
#define LIB_LOGIC_REGISTER_H


#include "wire.h"

namespace riscv_emu::logic {

class Register final {
 public:
  // TODO: When CPU is pipelined, return `out_` instead.
  Wire GetOut() const { return in_;}
  void SetIn(const Wire wire) { in_ = wire; }
  void Tick() { out_ = in_; }

 private:
  Wire in_, out_;
};

}  // namespace riscv_emu::logic

#endif  // LIB_LOGIC_REGISTER_H
