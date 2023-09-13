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
constexpr uint32_t kCsrMask = 0b111111111111 << kRs2Shift;

constexpr int kBytesInWord = 4;
constexpr uint32_t kByteMask = 0xff;

constexpr int kHalfWordsInWord = 2;
constexpr uint32_t kHalfWordMask = 0xffff;

}  // namespace constants

absl::StatusOr<uint8_t> GetByte(uint32_t val, size_t at_index);
absl::StatusOr<uint16_t> GetHalfWord(uint32_t val, size_t at_index);

absl::StatusOr<Opcode> GetOpcode(uint32_t val);
absl::StatusOr<uint32_t> GetFunc3(uint32_t val);
absl::StatusOr<uint32_t> GetFunc7(uint32_t val);
absl::StatusOr<uint32_t> GetFunc7Imm(uint32_t val);
absl::StatusOr<uint32_t> GetRs1(uint32_t val);
absl::StatusOr<uint32_t> GetRs2(uint32_t val);
absl::StatusOr<uint32_t> GetRd(uint32_t val);
absl::StatusOr<uint32_t> GetCsr(uint32_t val);

}  // namespace riscv_emu::logic

#endif  // LIB_LOGIC_WIRE_H
