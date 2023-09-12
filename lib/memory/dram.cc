#include "dram.h"
#include <iostream>
#include <fstream>
#include "glog/logging.h"
#include "status_macros.h"

namespace riscv_emu::memory {

  namespace {

    absl::StatusOr<bool> IsUnalignedAccess(const AccessType access_type, size_t at_index) {
      switch (access_type) {
       case AccessType::kByte:
       case AccessType::kByteUnsigned:
        return false;
       case AccessType::kHalfword:
       case AccessType::kHalfwordUnsigned:
        if (at_index  % sizeof(uint16_t) != 0) {
          return true;
        }
        break;
       case AccessType::kWord:
        if (at_index  % sizeof(uint32_t) != 0) {
          return true;
        }
        break;
       default:
        return absl::InternalError("Dram access type not properly set.");
      }
      return false;
    }

    logic::Wire ReadByte(const uint8_t* loc, const size_t at_index, const bool sign_extend) {
      const uint32_t val = loc[at_index];
      if (sign_extend & (val & 0b10000000)) {
        return logic::Wire(0xffffff00 | val);
      } else {
        return logic::Wire(val);
      }
    }

    logic::Wire ReadHalfWord(const uint8_t* loc, const size_t at_index, const bool sign_extend) {
      const uint32_t val = (loc[at_index + 1] << 8) | loc[at_index];
      if (sign_extend & (val & 0b1000000000000000)) {
        return logic::Wire(0xffff0000 | val);
      } else {
        return logic::Wire(val);
      }
    }

    logic::Wire ReadWord(const uint8_t* loc, const size_t at_index) {
      const uint32_t val = (loc[at_index + 3] << 24) |
                           (loc[at_index + 2] << 16) |
                           (loc[at_index + 1] << 8)  |
                            loc[at_index];
      return logic::Wire(val);
    }

    void WriteByte(uint8_t* loc, const size_t at_index, const logic::Wire val) {
      loc[at_index] = val.GetByte(0).value();
    }

    void WriteHalfword(uint8_t* loc, const size_t at_index, const logic::Wire val) {
      loc[at_index] = val.GetByte(0).value();
      loc[at_index + 1] = val.GetByte(1).value();
    }

    void WriteWord(uint8_t* loc, const size_t at_index, const logic::Wire val) {
      loc[at_index] = val.GetByte(0).value();
      loc[at_index + 1] = val.GetByte(1).value();
      loc[at_index + 2] = val.GetByte(2).value();
      loc[at_index + 3] = val.GetByte(3).value();
    }

  }  // namespace

absl::StatusOr<logic::Wire> Dram::Read(const size_t at_index) {
  if (at_index >= constants::kDramSize) {
    return absl::OutOfRangeError("Memory address out of range");
  }
  ASSIGN_OR_RETURN(const bool isUnaligned, IsUnalignedAccess(access_type_, at_index));
  if (isUnaligned) {
    return absl::FailedPreconditionError("unaligned memory access detected");
  }
  switch (access_type_) {
   case AccessType::kByte:
    return ReadByte(data_.get(), at_index, /*signed=*/true);
   case AccessType::kByteUnsigned:
    return ReadByte(data_.get(), at_index, /*signed=*/false);
   case AccessType::kHalfword:
    return ReadHalfWord(data_.get(), at_index, /*signed=*/true);
   case AccessType::kHalfwordUnsigned:
    return ReadHalfWord(data_.get(), at_index, /*signed=*/false);
   case AccessType::kWord:
    return ReadWord(data_.get(), at_index);
   default:
    return absl::InternalError("Dram access type not properly set.");
  }
}

absl::Status Dram::Write(const size_t at_index, const logic::Wire val) {
  if (at_index >= constants::kDramSize) {
    return absl::OutOfRangeError("Memory address out of range");
  }
  ASSIGN_OR_RETURN(const bool isUnaligned, IsUnalignedAccess(access_type_, at_index));
  if (isUnaligned) {
    return absl::FailedPreconditionError("Unaligned memory access detected");
  }
  switch (access_type_) {
   case AccessType::kByte:
   case AccessType::kByteUnsigned:
    WriteByte(data_.get(), at_index, val);
    return absl::OkStatus();
   case AccessType::kHalfword:
   case AccessType::kHalfwordUnsigned:
    WriteHalfword(data_.get(), at_index, val);
    return absl::OkStatus();
   case AccessType::kWord:
    WriteWord(data_.get(), at_index, val);
    return absl::OkStatus();
   default:
    return absl::InternalError("`Dram access type not properly set.");
  }
}

absl::Status Dram::Flash(const absl::string_view filename, const size_t at) {
  std::ifstream input_file(filename.data(), std::ios::in | std::ios::binary);
  if (!input_file.is_open()) {
    return absl::NotFoundError("Failed to open file");
  }

  size_t written = 0;
  char c = 0;
  while (input_file.get(c)) {
    LOG(INFO) << "WRITING BYTE 0x" << std::hex << (uint32_t)c << " to mem at 0x" << (at + written);
    if ((at + written) >= constants::kDramSize) {
      return absl::OutOfRangeError("disk image is larger than memory available");
    }
    data_[at + written] = static_cast<uint8_t>(c);
    written++;
  }
  input_file.close();

  return absl::OkStatus();
}

Dram::Dram() : data_(std::move(std::make_unique<uint8_t[]>(constants::kDramSize))) {}

}  // namespace riscv_emu::memory
