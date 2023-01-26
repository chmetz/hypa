#include "iss.h"

#include "../data_structures/options.h"
#include "../logger.h"
#include "../data_structures/string_helpers.h"

void register_dump(std::string func, const std::map<std::string, PtxRegister> &regs) {
  std::string output = func + " | Register Dump\n";

  for (const auto &r : regs) {
    output += "    ";
    output += r.first;
    output += ": ";
    output += r.second.ToString();
    output += "\n";
  }

  Logger::Log(output, "DEBUG");
}

int8_t Iss::CastWrapperS8(const PtxRegister &reg) {
  switch (reg.GetType()) {
    case kS8: return boost::any_cast<int8_t>(reg.GetValue());
    case kS16: return static_cast<int8_t>(boost::any_cast<int16_t>(reg.GetValue()));
    case kS32: return static_cast<int8_t>(boost::any_cast<int32_t>(reg.GetValue()));
    case kS64: return static_cast<int8_t>(boost::any_cast<int64_t>(reg.GetValue()));
    case kU8:
    case kB8: return static_cast<int8_t>(boost::any_cast<uint8_t>(reg.GetValue()));
    case kU16:
    case kB16: return static_cast<int8_t>(boost::any_cast<uint16_t>(reg.GetValue()));
    case kU32:
    case kB32: return static_cast<int8_t>(boost::any_cast<uint32_t>(reg.GetValue()));
    case kU64:
    case kB64: return static_cast<int8_t>(boost::any_cast<uint64_t>(reg.GetValue()));
    case kPred: return static_cast<int8_t>(boost::any_cast<bool>(reg.GetValue()));
    default: Logger::Log("Iss::CastWrapperS8 | Wrong conversion to int8_t\n", "ERROR");
  }
  exit(-1);
}

int16_t Iss::CastWrapperS16(const PtxRegister &reg) {
  switch (reg.GetType()) {
    case kS8: return static_cast<int16_t>(boost::any_cast<int8_t>(reg.GetValue()));
    case kS16: return boost::any_cast<int16_t>(reg.GetValue());
    case kS32: return static_cast<int16_t>(boost::any_cast<int32_t>(reg.GetValue()));
    case kS64: return static_cast<int16_t>(boost::any_cast<int64_t>(reg.GetValue()));
    case kU8:
    case kB8: return static_cast<int16_t>(boost::any_cast<uint8_t>(reg.GetValue()));
    case kU16:
    case kB16: return static_cast<int16_t>(boost::any_cast<uint16_t>(reg.GetValue()));
    case kU32:
    case kB32: return static_cast<int16_t>(boost::any_cast<uint32_t>(reg.GetValue()));
    case kU64:
    case kB64: return static_cast<int16_t>(boost::any_cast<uint64_t>(reg.GetValue()));
    case kPred: return static_cast<int16_t>(boost::any_cast<bool>(reg.GetValue()));
    default: Logger::Log("Iss::CastWrapperS16 | Wrong conversion to int16_t\n", "ERROR");
  }
  exit(-1);
}

int32_t Iss::CastWrapperS32(const PtxRegister &reg) {
  switch (reg.GetType()) {
    case kS8: return static_cast<int32_t>(boost::any_cast<int8_t>(reg.GetValue()));
    case kS16: return static_cast<int32_t>(boost::any_cast<int16_t>(reg.GetValue()));
    case kS32: return boost::any_cast<int32_t>(reg.GetValue());
    case kS64: return static_cast<int32_t>(boost::any_cast<int64_t>(reg.GetValue()));
    case kU8:
    case kB8: return static_cast<int32_t>(boost::any_cast<uint8_t>(reg.GetValue()));
    case kU16:
    case kB16: return static_cast<int32_t>(boost::any_cast<uint16_t>(reg.GetValue()));
    case kU32:
    case kB32: return static_cast<int32_t>(boost::any_cast<uint32_t>(reg.GetValue()));
    case kU64:
    case kB64: return static_cast<int32_t>(boost::any_cast<uint64_t>(reg.GetValue()));
    case kPred: return static_cast<int32_t>(boost::any_cast<bool>(reg.GetValue()));
    default: Logger::Log("Iss::CastWrapperS32 | Wrong conversion to int32_t\n", "ERROR");
  }
  exit(-1);
}

int64_t Iss::CastWrapperS64(const PtxRegister &reg) {
  switch (reg.GetType()) {
    case kS8: return static_cast<int64_t>(boost::any_cast<int8_t>(reg.GetValue()));
    case kS16: return static_cast<int64_t>(boost::any_cast<int16_t>(reg.GetValue()));
    case kS32: return static_cast<int64_t>(boost::any_cast<int32_t>(reg.GetValue()));
    case kS64: return boost::any_cast<int64_t>(reg.GetValue());
    case kU8:
    case kB8: return static_cast<int64_t>(boost::any_cast<uint8_t>(reg.GetValue()));
    case kU16:
    case kB16: return static_cast<int64_t>(boost::any_cast<uint16_t>(reg.GetValue()));
    case kU32:
    case kB32: return static_cast<int64_t>(boost::any_cast<uint32_t>(reg.GetValue()));
    case kU64:
    case kB64: return static_cast<int64_t>(boost::any_cast<uint64_t>(reg.GetValue()));
    case kPred: return static_cast<int64_t>(boost::any_cast<bool>(reg.GetValue()));
    default: Logger::Log("Iss::CastWrapperS64 | Wrong conversion to int64_t\n", "ERROR");
  }
  exit(-1);
}

uint8_t Iss::CastWrapperU8(const PtxRegister &reg) {
  switch (reg.GetType()) {
    case kS8: return static_cast<uint8_t>(boost::any_cast<int8_t>(reg.GetValue()));
    case kS16: return static_cast<uint8_t>(boost::any_cast<int16_t>(reg.GetValue()));
    case kS32: return static_cast<uint8_t>(boost::any_cast<int32_t>(reg.GetValue()));
    case kS64: return static_cast<uint8_t>(boost::any_cast<int64_t>(reg.GetValue()));
    case kU8:
    case kB8: return boost::any_cast<uint8_t>(reg.GetValue());
    case kU16:
    case kB16: return static_cast<uint8_t>(boost::any_cast<uint16_t>(reg.GetValue()));
    case kU32:
    case kB32: return static_cast<uint8_t>(boost::any_cast<uint32_t>(reg.GetValue()));
    case kU64:
    case kB64: return static_cast<uint8_t>(boost::any_cast<uint64_t>(reg.GetValue()));
    case kPred: return static_cast<uint8_t>(boost::any_cast<bool>(reg.GetValue()));
    default: Logger::Log("Iss::CastWrapperU8 | Wrong conversion to uint8_t\n", "ERROR");
  }
  exit(-1);
}

uint16_t Iss::CastWrapperUB16(const PtxRegister &reg) {
  switch (reg.GetType()) {
    case kS8: return static_cast<uint16_t>(boost::any_cast<int8_t>(reg.GetValue()));
    case kS16: return static_cast<uint16_t>(boost::any_cast<int16_t>(reg.GetValue()));
    case kS32: return static_cast<uint16_t>(boost::any_cast<int32_t>(reg.GetValue()));
    case kS64: return static_cast<uint16_t>(boost::any_cast<int64_t>(reg.GetValue()));
    case kU8:
    case kB8: return static_cast<uint16_t>(boost::any_cast<uint8_t>(reg.GetValue()));
    case kU16:
    case kB16: return boost::any_cast<uint16_t>(reg.GetValue());
    case kU32:
    case kB32: return static_cast<uint16_t>(boost::any_cast<uint32_t>(reg.GetValue()));
    case kU64:
    case kB64: return static_cast<uint16_t>(boost::any_cast<uint64_t>(reg.GetValue()));
    case kPred: return static_cast<uint16_t>(boost::any_cast<bool>(reg.GetValue()));
    default: Logger::Log("Iss::CastWrapperUB16 | Wrong conversion to uint16_t\n", "ERROR");
  }
  exit(-1);
}

uint32_t Iss::CastWrapperUB32(const PtxRegister &reg) {
  switch (reg.GetType()) {
    case kS8: return static_cast<uint32_t>(boost::any_cast<int8_t>(reg.GetValue()));
    case kS16: return static_cast<uint32_t>(boost::any_cast<int16_t>(reg.GetValue()));
    case kS32: return static_cast<uint32_t>(boost::any_cast<int32_t>(reg.GetValue()));
    case kS64: return static_cast<uint32_t>(boost::any_cast<int64_t>(reg.GetValue()));
    case kU8:
    case kB8: return static_cast<uint32_t>(boost::any_cast<uint8_t>(reg.GetValue()));
    case kU16:
    case kB16: return static_cast<uint32_t>(boost::any_cast<uint16_t>(reg.GetValue()));
    case kU32:
    case kB32: return boost::any_cast<uint32_t>(reg.GetValue());
    case kU64:
    case kB64: return static_cast<uint32_t>(boost::any_cast<uint64_t>(reg.GetValue()));
    case kPred: return static_cast<uint32_t>(boost::any_cast<bool>(reg.GetValue()));
    default: Logger::Log("Iss::CastWrapperUB32 | Wrong conversion to uint32_t\n", "ERROR");
  }
  exit(-1);
}

uint64_t Iss::CastWrapperUB64(const PtxRegister &reg) {
  switch (reg.GetType()) {
    case kS8: return static_cast<uint64_t>(boost::any_cast<int8_t>(reg.GetValue()));
    case kS16: return static_cast<uint64_t>(boost::any_cast<int16_t>(reg.GetValue()));
    case kS32: return static_cast<uint64_t>(boost::any_cast<int32_t>(reg.GetValue()));
    case kS64: return static_cast<uint64_t>(boost::any_cast<int64_t>(reg.GetValue()));
    case kU8:
    case kB8: return static_cast<uint64_t>(boost::any_cast<uint8_t>(reg.GetValue()));
    case kU16:
    case kB16: return static_cast<uint64_t>(boost::any_cast<uint16_t>(reg.GetValue()));
    case kU32:
    case kB32: return static_cast<uint64_t>(boost::any_cast<uint32_t>(reg.GetValue()));
    case kU64:
    case kB64: return boost::any_cast<uint64_t>(reg.GetValue());
    case kPred: return static_cast<uint64_t>(boost::any_cast<bool>(reg.GetValue()));
    default: Logger::Log("Iss::CastWrapperUB64 | Wrong conversion to uint64_t\n", "ERROR");
  }
  exit(-1);
}

bool Iss::CastWrapperPred(const PtxRegister &reg) {
  switch (reg.GetType()) {
    case kS8: return static_cast<bool>(boost::any_cast<int8_t>(reg.GetValue()));
    case kS16: return static_cast<bool>(boost::any_cast<int16_t>(reg.GetValue()));
    case kS32: return static_cast<bool>(boost::any_cast<int32_t>(reg.GetValue()));
    case kS64: return static_cast<bool>(boost::any_cast<int64_t>(reg.GetValue()));
    case kU8:
    case kB8: return static_cast<bool>(boost::any_cast<uint8_t>(reg.GetValue()));
    case kU16:
    case kB16: return static_cast<bool>(boost::any_cast<uint16_t>(reg.GetValue()));
    case kU32:
    case kB32: return static_cast<bool>(boost::any_cast<uint32_t>(reg.GetValue()));
    case kU64:
    case kB64: return static_cast<bool>(boost::any_cast<uint64_t>(reg.GetValue()));
    case kPred: return boost::any_cast<bool>(reg.GetValue());
    default: Logger::Log("Iss::CastWrapperPred | Wrong conversion to bool\n", "ERROR");
  }
  exit(-1);
}

std::map<std::string, PtxRegister> &Iss::Execute(const std::set<PtxInstruction> &instructions,
                                                 std::map<std::string, PtxRegister> &regs) {

  for (const PtxInstruction &i : instructions) {
    if (Options::verbose) {
      Logger::Log(
          "Iss::Execute | Starting to Execute: \"" + i.GetRawLine() + "\" ID: " + std::to_string(i.GetId()) + "\n",
          "TRACE");
    }

    if (i.HasGuard() && CastWrapperS64(regs[i.GetGuardRegister()]) == i.GetGuardMod()) {
      continue;
    }

    for (const auto &r : i.GetSrcRegister()) {
      if (r[0] != '%') {
        if (r[0] == '[') {
          size_t pos_plus = r.find('+');
          size_t pos_minus = r.find('-');
          if (pos_plus != std::string::npos) {
            Logger::Log("Iss::Execute | Reg: " + r.substr(1, pos_plus - 1) + " Offset (+): "
                            + r.substr(pos_plus + 1, r.size() - pos_plus - 2) + "\n", "DEBUG");
          } else if (pos_minus != std::string::npos) {
            Logger::Log("Iss::Execute | Reg: " + r.substr(1, pos_minus - 1) + " Offset (-): "
                            + r.substr(pos_minus + 1, r.size() - pos_minus - 2) + "\n", "DEBUG");
          } else {
            Logger::Log("Iss::Execute | Reg: " + r.substr(1, r.size() - 2) + "\n", "DEBUG");
          }
        } else {
          regs[r] = PtxRegister(kU64, static_cast<uint64_t>(std::stoi(r)));
        }
      }
    }

    switch (i.GetInstructionType()) {
      case kU8: U8Iss(i, regs);
        break;
      case kU16:
      case kB16: UB16Iss(i, regs);
        break;
      case kU32:
      case kB32: UB32Iss(i, regs);
        break;
      case kU64:
      case kB64: UB64Iss(i, regs);
        break;
      case kS8: S8Iss(i, regs);
        break;
      case kS16: S16Iss(i, regs);
        break;
      case kS32: S32Iss(i, regs);
        break;
      case kS64: S64Iss(i, regs);
        break;
      case kPred: PredIss(i, regs);
        break;
      case kNoType: Logger::Log("Iss::Execute | Instruction has no type: \"" + i.GetRawLine() + "\"\n", "DEBUG");
        break;
      default: Logger::Log("Iss::Execute | No Iss for type: " + TypeToString[i.GetInstructionType()] + "\n", "ERROR");
    }
  }

  return regs;
}

