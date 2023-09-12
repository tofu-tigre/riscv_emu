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
        // Should never occur.
        return absl::InternalError("`HasFunc3()` recieved an invalid opcode as an argument");
      }
    }

    absl::StatusOr<bool> HasFunc7(const Opcode opcode) {
      switch (opcode) {
       case Opcode::kRType:
        return true;
       case Opcode::kIType:
       case Opcode::kLType:
       case Opcode::kJalrType:
       case Opcode::kSType:
       case Opcode::kBType:
       case Opcode::kAuiPcType:
       case Opcode::kLuiType:
       case Opcode::kJalType:
        return false;
       default:
        // Should never occur.
        return absl::InternalError("`HasFunc7()` recieved an invalid opcode as an argument");
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
       case Opcode::kJalType:
        return false;
       default:
        // Should never occur.
        return absl::InternalError("`HasRs1()` recieved an invalid opcode as an argument");
      }
    }

    absl::StatusOr<bool> HasRs2(const Opcode opcode) {
      switch (opcode) {
       case Opcode::kRType:
       case Opcode::kSType:
       case Opcode::kBType:
        return true;
       case Opcode::kIType:
       case Opcode::kLType:
       case Opcode::kJalrType:
       case Opcode::kAuiPcType:
       case Opcode::kLuiType:
       case Opcode::kJalType:
        return false;
       default:
        // Should never occur.
        return absl::InternalError("`HasFunc3()` recieved an invalid opcode as an argument");
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
        return false;
       default:
        // Should never occur.
        return absl::InternalError("`HasFunc3()` recieved an invalid opcode as an argument");
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
     default:
      return absl::InvalidArgumentError("No opcode found");
    }
  }

  absl::StatusOr<uint32_t> Wire::GetFunc3() const {
    ASSIGN_OR_RETURN(const Opcode opcode, this->GetOpcode());
    ASSIGN_OR_RETURN(const bool hasFunc3, HasFunc3(opcode));
    if (!hasFunc3) {
      return absl::FailedPreconditionError("Instruction does not contain a func3 field");
    }
    return (value_.u32 & constants::kFunc3Mask) >> constants::kFunc3Shift;
  }

  absl::StatusOr<uint32_t> Wire::GetFunc7() const {
    ASSIGN_OR_RETURN(const Opcode opcode, this->GetOpcode());
    ASSIGN_OR_RETURN(const bool hasFunc7, HasFunc7(opcode));
    if (!hasFunc7) {
      return absl::FailedPreconditionError("Instruction does not contain a func7 field");
    }
    return (value_.u32 & constants::kFunc7Mask) >> constants::kFunc7Shift;
  }

  absl::StatusOr<uint32_t> Wire::GetFunc7Imm() const {
    ASSIGN_OR_RETURN(const Opcode opcode, this->GetOpcode());
    if (opcode != Opcode::kIType) {
      return absl::FailedPreconditionError("Instruction does not contain a func7-imm field");
    }
    return (value_.u32 & constants::kFunc7Mask) >> constants::kFunc7Shift;
  }

  absl::StatusOr<uint32_t> Wire::GetRd() const {
    // Ensure that the wire opcode contains a func3 field.
    ASSIGN_OR_RETURN(const Opcode opcode, this->GetOpcode());
    ASSIGN_OR_RETURN(const bool hasRd, HasRd(opcode));
    if (!hasRd) {
      return absl::FailedPreconditionError("Instruction does not contain a rd field");
    }
    return (value_.u32 & constants::kRdMask) >> constants::kRdShift;
  }

  absl::StatusOr<uint32_t> Wire::GetRs1() const {
    // Ensure that the wire opcode contains a func3 field.
    ASSIGN_OR_RETURN(const Opcode opcode, this->GetOpcode());
    ASSIGN_OR_RETURN(const bool hasRs1, HasRs1(opcode));
    if (!hasRs1) {
      return absl::FailedPreconditionError("Instruction does not contain a rs1 field");
    }
    return (value_.u32 & constants::kRs1Mask) >> constants::kRs1Shift;
  }

  absl::StatusOr<uint32_t> Wire::GetRs2() const {
    // Ensure that the wire opcode contains a func3 field.
    ASSIGN_OR_RETURN(const Opcode opcode, this->GetOpcode());
    ASSIGN_OR_RETURN(const bool hasRs2, HasRs2(opcode));
    if (!hasRs2) {
      return absl::FailedPreconditionError("Instruction does not contain a rs2 field");
    }
    return (value_.u32 & constants::kRs2Mask) >> constants::kRs2Shift;
  }

  absl::StatusOr<uint8_t> Wire::GetByte(size_t at_index) const {
    if (at_index >= constants::kBytesInWord) {
      return absl::InvalidArgumentError("Request byte at invalid index");
    }
    constexpr int kBitsInAByte = 8;
    return (value_.u32 & (constants::kByteMask << (at_index * kBitsInAByte))) >> (at_index * kBitsInAByte);
  }

  absl::StatusOr<uint16_t> Wire::GetHalfWord(size_t at_index) const {
    if (at_index >= constants::kHalfWordsInWord) {
      return absl::InvalidArgumentError("Request halfword at invalid index");
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
