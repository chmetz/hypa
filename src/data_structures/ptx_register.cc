#include "ptx_register.h"

#include "../data_structures/string_helpers.h"

std::ostream &operator<<(std::ostream &out, const PtxRegister &p) {
  out << "<PTX Register[type: " << TypeToString[p.GetType()] << ",value: ";
  switch (p.GetType()) {
    case kS8: out << boost::any_cast<int8_t>(p.GetValue());
      break;
    case kS16: out << boost::any_cast<int16_t>(p.GetValue());
      break;
    case kS32: out << boost::any_cast<int32_t>(p.GetValue());
      break;
    case kS64: out << boost::any_cast<int64_t>(p.GetValue());
      break;
    case kU8:
    case kB8: out << boost::any_cast<uint8_t>(p.GetValue());
      break;
    case kU16:
    case kB16: out << boost::any_cast<uint16_t>(p.GetValue());
      break;
    case kU32:
    case kB32: out << boost::any_cast<uint32_t>(p.GetValue());
      break;
    case kU64:
    case kB64: out << boost::any_cast<uint64_t>(p.GetValue());
      break;
    case kPred: out << (boost::any_cast<bool>(p.GetValue()) ? "true" : "false");
      break;
    default:out << " \033[33m{default cast}\033[0m ";
      out << boost::any_cast<int>(p.GetValue());
  }
  out << "]>";
  return out;
}

PtxRegister::PtxRegister() {
  type_ = kNoType;
}

PtxRegister::PtxRegister(Type type) {
  type_ = type;
}

PtxRegister::PtxRegister(Type type, boost::any value) {
  type_ = type;
  value_ = value;
}

void PtxRegister::SetType(Type type) {
  type_ = type;
}

Type PtxRegister::GetType() const {
  return type_;
}

void PtxRegister::SetValue(boost::any value) {
  value_ = value;
}

boost::any PtxRegister::GetValue() const {
  return value_;
}

std::string PtxRegister::ToString() const {
  std::string out = "<PTX Register[type: " + TypeToString[GetType()] + ",value: ";

  switch (GetType()) {
    case kS8: out += boost::any_cast<int8_t>(GetValue());
      break;
    case kS16: out += std::to_string(boost::any_cast<int16_t>(GetValue()));
      break;
    case kS32: out += std::to_string(boost::any_cast<int32_t>(GetValue()));
      break;
    case kS64: out += std::to_string(boost::any_cast<int64_t>(GetValue()));
      break;
    case kU8:
    case kB8: out += std::to_string(boost::any_cast<uint8_t>(GetValue()));
      break;
    case kU16:
    case kB16: out += std::to_string(boost::any_cast<uint16_t>(GetValue()));
      break;
    case kU32:
    case kB32: out += std::to_string(boost::any_cast<uint32_t>(GetValue()));
      break;
    case kU64:
    case kB64: out += std::to_string(boost::any_cast<uint64_t>(GetValue()));
      break;
    case kPred: out += (boost::any_cast<bool>(GetValue()) ? "true" : "false");
      break;
    default: out += " {default cast} ";
      out += std::to_string(boost::any_cast<int>(GetValue()));
  }
  out += "]>";
  return out;
}

