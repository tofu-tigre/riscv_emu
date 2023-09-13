#include "wire.h"

#include <bitset>
#include "status_macros.h"
#include "glog/logging.h"

namespace riscv_emu::logic {

namespace {

absl::StatusOr<bool> HasFunc3(const Opcode opcode) {
  switch (opcode) {
   case Opcode::kRType:
   case Opcode::kIType:
   case Opcode::kJalrType:
   case Opcode::kLType:
   case Opcode::kSType:
   case Opcode::kBType:
    return true;
   case Opcode::kLuiType:
   case Opcode::kAuiPcType:
   case Opcode::kJalType:
    return false;
   default:
    return absl::InternalError("Invalid opcode found");
  }
}

absl::StatusOr<bool> HasFunc7(const Opcode opcode) {
  switch (opcode) {
   case Opcode::kRType:
   case Opcode::kIType:
    return true;
   case Opcode::kLType:
   case Opcode::kJalrType:
   case Opcode::kSType:
   case Opcode::kBType:
   case Opcode::kAuiPcType:
   case Opcode::kLuiType:
   case Opcode::kEType:
   case Opcode::kJalType:
    return false;
   default:
    return absl::InternalError("Invalid opcode found");
  }
}

absl::StatusOr<bool> HasRs1(const Opcode opcode) {
  switch (opcode) {
   case Opcode::kRType:
   case Opcode::kIType:
   case Opcode::kJalrType:
   case Opcode::kLType:
   case Opcode::kSType:
   case Opcode::kBType:
    return true;
   case Opcode::kLuiType:
   case Opcode::kAuiPcType:
   case Opcode::kEType:
   case Opcode::kJalType:
    return false;
   default:
    return absl::InternalError("Invalid opcode found");
  }
}

absl::StatusOr<bool> HasRs2(const Opcode opcode) {
  switch (opcode) {
   case Opcode::kRType:
   case Opcode::kSType:
   case Opcode::kBType:
   case Opcode::kEType:
    return true;
   case Opcode::kIType:
   case Opcode::kLType:
   case Opcode::kJalrType:
   case Opcode::kAuiPcType:
   case Opcode::kLuiType:
   case Opcode::kJalType:
   case Opcode::kEType:
    return false;
   default:
    return absl::InternalError("Invalid opcode found");
  }
}

absl::StatusOr<bool> HasRd(const Opcode opcode) {
  switch (opcode) {
   case Opcode::kRType:
   case Opcode::kIType:
   case Opcode::kLType:
   case Opcode::kLuiType:
   case Opcode::kAuiPcType:
   case Opcode::kJalType:
   case Opcode::kJalrType:
    return true;
   case Opcode::kSType:
   case Opcode::kBType:
   case Opcode::kEType:
    return false;
   default:
    return absl::InternalError("Invalid opcode found");
  }
}

}  // namespace

absl::StatusOr<Opcode> Wire::GetOpcode() const {
  const uint32_t opcode = value_.u32 & constants::kOpcodeMask;
  switch (opcode) {
   case static_cast<uint32_t>(Opcode::kRType):
    return Opcode::kRType;
   case static_cast<uint32_t>(Opcode::kIType):
    return Opcode::kIType;
   case static_cast<uint32_t>(Opcode::kSType):
    return Opcode::kSType;
   case static_cast<uint32_t>(Opcode::kBType):
    return Opcode::kBType;
   case static_cast<uint32_t>(Opcode::kLuiType):
    return Opcode::kLuiType;
   case static_cast<uint32_t>(Opcode::kAuiPcType):
    return Opcode::kAuiPcType;
   case static_cast<uint32_t>(Opcode::kJalType):
    return Opcode::kJalType;
   case static_cast<uint32_t>(Opcode::kJalrType):
    return Opcode::kJalrType;
   case static_cast<uint32_t>(Opcode::kLType):
    return Opcode::kLType;
   case static_cast<uint32_t>(Opcode::kEType):
    return Opcode::kEType;
   default:
    return absl::NotFoundError("No valid opcode found in instruction");
  }
}

absl::StatusOr<uint32_t> Wire::GetFunc3() const {
  ASSIGN_OR_RETURN(const Opcode opcode, this->GetOpcode());
  ASSIGN_OR_RETURN(const bool hasFunc3, HasFunc3(opcode));
  if (!hasFunc3) {
    return absl::NotFoundError("Instruction does not contain a func3 field");
  }
  return (value_.u32 & constants::kFunc3Mask) >> constants::kFunc3Shift;
}

absl::StatusOr<uint32_t> Wire::GetFunc7() const {
  ASSIGN_OR_RETURN(const Opcode opcode, this->GetOpcode());
  ASSIGN_OR_RETURN(const bool hasFunc7, HasFunc7(opcode));
  if (!hasFunc7) {
    return absl::NotFoundError("Instruction does not contain a func7 field");
  }
  return (value_.u32 & constants::kFunc7Mask) >> constants::kFunc7Shift;
}

absl::StatusOr<uint32_t> Wire::GetRd() const {
  ASSIGN_OR_RETURN(const Opcode opcode, this->GetOpcode());
  ASSIGN_OR_RETURN(const bool hasRd, HasRd(opcode));
  if (!hasRd) {
    return absl::NotFoundError("Instruction does not contain a rd field");
  }
  return (value_.u32 & constants::kRdMask) >> constants::kRdShift;
}

absl::StatusOr<uint32_t> Wire::GetRs1() const {
  ASSIGN_OR_RETURN(const Opcode opcode, this->GetOpcode());
  ASSIGN_OR_RETURN(const bool hasRs1, HasRs1(opcode));
  if (!hasRs1) {
    return absl::NotFoundError("Instruction does not contain a rs1 field");
  }
  return (value_.u32 & constants::kRs1Mask) >> constants::kRs1Shift;
}

absl::StatusOr<uint32_t> Wire::GetRs2() const {
  ASSIGN_OR_RETURN(const Opcode opcode, this->GetOpcode());
  ASSIGN_OR_RETURN(const bool hasRs2, HasRs2(opcode));
  if (!hasRs2) {
    return absl::NotFoundError("Instruction does not contain a rs2 field");
  }
  return (value_.u32 & constants::kRs2Mask) >> constants::kRs2Shift;
}

absl::StatusOr<uint8_t> Wire::GetByte(size_t at_index) const {
  if (at_index >= constants::kBytesInWord) {
    return absl::InvalidArgumentError("Requested byte at invalid index");
  }
  constexpr int kBitsInAByte = 8;
  return (value_.u32 & (constants::kByteMask << (at_index * kBitsInAByte))) >> (at_index * kBitsInAByte);
}

absl::StatusOr<uint16_t> Wire::GetHalfWord(size_t at_index) const {
  if (at_index >= constants::kHalfWordsInWord) {
    return absl::InvalidArgumentError("Requested halfword at invalid index");
  }
  constexpr int kBitsInAHalfWord = 16;
  return (value_.u32 & (constants::kByteMask << (at_index * kBitsInAHalfWord))) >> (at_index * kBitsInAHalfWord);
}

std::ostream& operator<<(std::ostream& os, const Wire& wire) {
  std::bitset<32> val{wire.value_.u32};
  os << "0b" << val;
  return os;
}

} // namespace riscv_emu::logic
