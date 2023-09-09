#ifndef LIB_LOGIC_WIRE_H
#define LIB_LOGIC_WIRE_H

#include <stdint.h>
#include <iostream>
#include "lib/logic/opcodes.h"
#include "absl/status/statusor.h"

namespace riscv_emu::logic {

  namespace constants {

    constexpr int kFunc3Shift = 12;
    constexpr int kFunc7Shift = 25;
    constexpr int kRdShift = 7;
    constexpr int kRs1Shift = 15;
    constexpr int kRs2Shift = 20;

    constexpr uint32_t kFunc3Mask = 0b111 << kFunc3Shift;
    constexpr uint32_t kFunc7Mask = 0b1111111 << kFunc7Shift;
    constexpr uint32_t kRdMask = 0b11111 << kRdShift;
    constexpr uint32_t kRs1Mask = 0b11111 << kRs1Shift;
    constexpr uint32_t kRs2Mask = 0b11111 << kRs2Shift; 

  }  // namespace constants

  class Wire final {
   private:
    union {
      uint32_t u32;
      int32_t i32;
    } value_;

   public:
    // TODO: Consider making these private and forcing use
    // of a `Wire::CreateUnsigned()` / `Wire::CreateSigned()` method?
    explicit Wire(uint32_t value) { value_.u32 = value; }
    explicit Wire(int32_t value) { value_.i32 = value; }
    Wire() { value_.u32 = 0; }

    inline void SetUnsigned(uint32_t value) { value_.u32 = value; }
    inline void SetSigned(int32_t value) { value_.i32 = value; }
    inline uint32_t GetUnsigned() const { return value_.u32; }
    inline int32_t GetSigned() const { return value_.i32; }

    absl::StatusOr<Opcode> GetOpcode() const;
    absl::StatusOr<uint32_t> GetFunc3() const;
    absl::StatusOr<uint32_t> GetFunc7() const;
    absl::StatusOr<uint32_t> GetRs1() const;
    absl::StatusOr<uint32_t> GetRs2() const;
    absl::StatusOr<uint32_t> GetRd() const;

    friend std::ostream& operator<<(std::ostream& os, const Wire& dt);
    
  };

}  // namespace riscv_emu::logic

#endif  // LIB_LOGIC_WIRE_H
