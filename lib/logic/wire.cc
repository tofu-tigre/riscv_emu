#include "wire.h"

#include <bitset>
#include "status_macros.h"

namespace riscv_emu::logic {

  namespace {

    absl::StatusOr<bool> HasFunc3(const Opcode opcode) {
      switch (opcode) {
       case Opcode::R_TYPE:
       case Opcode::I_TYPE:
       case Opcode::S_TYPE:
       case Opcode::B_TYPE:
        return true;
       case Opcode::U_TYPE:
       case Opcode::J_TYPE:
        return false;
       default:
        // Should never occur.
        return absl::InternalError("`HasFunc3()` recieved an invalid opcode as an argument");
      }
    }

    absl::StatusOr<bool> HasFunc7(const Opcode opcode) {
      switch (opcode) {
       case Opcode::R_TYPE:
        return true;
       case Opcode::I_TYPE:
       case Opcode::S_TYPE:
       case Opcode::B_TYPE:
       case Opcode::U_TYPE:
       case Opcode::J_TYPE:
        return false;
       default:
        // Should never occur.
        return absl::InternalError("`HasFunc7()` recieved an invalid opcode as an argument");
      }
    }

    absl::StatusOr<bool> HasRs1(const Opcode opcode) {
      switch (opcode) {
       case Opcode::R_TYPE:
       case Opcode::I_TYPE:
       case Opcode::S_TYPE:
       case Opcode::B_TYPE:
        return true;
       case Opcode::U_TYPE:
       case Opcode::J_TYPE:
        return false;
       default:
        // Should never occur.
        return absl::InternalError("`HasRs1()` recieved an invalid opcode as an argument");
      }
    }

    absl::StatusOr<bool> HasRs2(const Opcode opcode) {
      switch (opcode) {
       case Opcode::R_TYPE:
       case Opcode::S_TYPE:
       case Opcode::B_TYPE:
        return true;
       case Opcode::I_TYPE:
       case Opcode::U_TYPE:
       case Opcode::J_TYPE:
        return false;
       default:
        // Should never occur.
        return absl::InternalError("`HasFunc3()` recieved an invalid opcode as an argument");
      }
    }

    absl::StatusOr<bool> HasRd(const Opcode opcode) {
      switch (opcode) {
       case Opcode::R_TYPE:
       case Opcode::I_TYPE:
       case Opcode::U_TYPE:
       case Opcode::J_TYPE:
        return true;
       case Opcode::S_TYPE:
       case Opcode::B_TYPE:
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
     case static_cast<uint32_t>(Opcode::R_TYPE):
      return Opcode::R_TYPE;
      break;
     case static_cast<uint32_t>(Opcode::I_TYPE):
      return Opcode::I_TYPE;
      break;
     case static_cast<uint32_t>(Opcode::S_TYPE):
      return Opcode::S_TYPE;
      break;
     case static_cast<uint32_t>(Opcode::B_TYPE):
      return Opcode::B_TYPE;
      break;
     case static_cast<uint32_t>(Opcode::U_TYPE):
      return Opcode::U_TYPE;
      break;
     case static_cast<uint32_t>(Opcode::J_TYPE):
      return Opcode::J_TYPE;
      break;
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

  std::ostream& operator<<(std::ostream& os, const Wire& wire) {
    std::bitset<32> val{wire.value_.u32};
    os << "0b" << val;
    return os;
  }

} // namespace riscv_emu::logic
