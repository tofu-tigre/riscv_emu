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
   case Opcode::kEType:
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
    return false;
   default:
    return absl::InternalError("Invalid opcode found");
  }
}

absl::StatusOr<bool> HasCsr(const Opcode opcode) {
  switch (opcode) {
   case Opcode::kEType:
    return true;
   case Opcode::kRType:
   case Opcode::kSType:
   case Opcode::kBType:
   case Opcode::kIType:
   case Opcode::kLType:
   case Opcode::kJalrType:
   case Opcode::kAuiPcType:
   case Opcode::kLuiType:
   case Opcode::kJalType:
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

absl::StatusOr<Opcode> GetOpcode(const uint32_t val) {
  const uint32_t opcode = val & constants::kOpcodeMask;
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

absl::StatusOr<uint32_t> GetFunc3(const uint32_t val) {
  ASSIGN_OR_RETURN(const Opcode opcode, GetOpcode(val));
  ASSIGN_OR_RETURN(const bool has_func3, HasFunc3(opcode));
  if (!has_func3) {
    return absl::NotFoundError("Instruction does not contain a func3 field");
  }
  return (val & constants::kFunc3Mask) >> constants::kFunc3Shift;
}

absl::StatusOr<uint32_t> GetFunc7(const uint32_t val) {
  ASSIGN_OR_RETURN(const Opcode opcode, GetOpcode(val));
  ASSIGN_OR_RETURN(const bool has_func7, HasFunc7(opcode));
  if (!has_func7) {
    return absl::NotFoundError("Instruction does not contain a func7 field");
  }
  return (val & constants::kFunc7Mask) >> constants::kFunc7Shift;
}

absl::StatusOr<uint32_t> GetCsr(const uint32_t val) {
  ASSIGN_OR_RETURN(const Opcode opcode, GetOpcode(val));
  ASSIGN_OR_RETURN(const bool has_csr, HasCsr(opcode));
  if (!has_csr) {
    return absl::NotFoundError("Instruction does not contain a csr field");
  }
  return (val & constants::kCsrMask) >> constants::kRs2Shift;
}

absl::StatusOr<uint32_t> GetRd(const uint32_t val) {
  ASSIGN_OR_RETURN(const Opcode opcode, GetOpcode(val));
  ASSIGN_OR_RETURN(const bool has_rd, HasRd(opcode));
  if (!has_rd) {
    return absl::NotFoundError("Instruction does not contain a rd field");
  }
  return (val & constants::kRdMask) >> constants::kRdShift;
}

absl::StatusOr<uint32_t> GetRs1(const uint32_t val) {
  ASSIGN_OR_RETURN(const Opcode opcode, GetOpcode(val));
  ASSIGN_OR_RETURN(const bool has_rs1, HasRs1(opcode));
  if (!has_rs1) {
    return absl::NotFoundError("Instruction does not contain a rs1 field");
  }
  return (val & constants::kRs1Mask) >> constants::kRs1Shift;
}

absl::StatusOr<uint32_t> GetRs2(const uint32_t val) {
  ASSIGN_OR_RETURN(const Opcode opcode, GetOpcode(val));
  ASSIGN_OR_RETURN(const bool has_rs2, HasRs2(opcode));
  if (!has_rs2) {
    return absl::NotFoundError("Instruction does not contain a rs2 field");
  }
  return (val & constants::kRs2Mask) >> constants::kRs2Shift;
}

absl::StatusOr<uint8_t> GetByte(const uint32_t val, const size_t at_index) {
  if (at_index >= constants::kBytesInWord) {
    return absl::InvalidArgumentError("Requested byte at invalid index");
  }
  constexpr int kBitsInAByte = 8;
  return (val & (constants::kByteMask << (at_index * kBitsInAByte))) >> (at_index * kBitsInAByte);
}

absl::StatusOr<uint16_t> GetHalfWord(const uint32_t val, const size_t at_index) {
  if (at_index >= constants::kHalfWordsInWord) {
    return absl::InvalidArgumentError("Requested halfword at invalid index");
  }
  constexpr int kBitsInAHalfWord = 16;
  return (val & (constants::kByteMask << (at_index * kBitsInAHalfWord))) >> (at_index * kBitsInAHalfWord);
}

} // namespace riscv_emu::logic
