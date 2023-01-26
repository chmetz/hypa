#include <bitset>
#include "iss.h"
#include "../logger.h"

std::map<std::string, PtxRegister> &Iss::UB32Iss(const PtxInstruction &i, std::map<std::string, PtxRegister> &regs) {
  uint32_t src0;
  uint32_t src1;
  uint32_t src2;
  uint32_t dst;
  std::bitset<32> bs0;
  std::bitset<32> bs1;

  uint64_t double_size_dst;
  bool pred;

  switch (i.GetInstructionKeyword()) {
    // Integer Arithmetic Instructions
    case kAdd: src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB32(regs[i.GetSrcRegister()[1]]);

      dst = src0 + src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU32);
      break;
    case kSub: src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB32(regs[i.GetSrcRegister()[1]]);

      dst = src0 - src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU32);
      break;
    case kMul: src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB32(regs[i.GetSrcRegister()[1]]);

      double_size_dst = static_cast<uint64_t>(src0) * static_cast<uint64_t>(src1);
      if (i.GetModifier()[0] == "hi") {
        regs[i.GetDstRegister()].SetValue(static_cast<uint32_t>(double_size_dst >> 32));
        regs[i.GetDstRegister()].SetType(kU32);
      } else if (i.GetModifier()[0] == "lo") {
        regs[i.GetDstRegister()].SetValue(static_cast<uint32_t>(double_size_dst & 4294967295));
        regs[i.GetDstRegister()].SetType(kU32);
      } else if (i.GetModifier()[0] == "wide") {
        regs[i.GetDstRegister()].SetValue(double_size_dst);
        regs[i.GetDstRegister()].SetType(kU64);
      } else {
        Logger::Log("Iss::UB32Iss | Instruction has no valid modifier \"" + i.GetRawLine() + "\"\n", "ERROR");
      }
      break;
    case kMad: src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB32(regs[i.GetSrcRegister()[1]]);
      src2 = CastWrapperUB32(regs[i.GetSrcRegister()[2]]);

      double_size_dst = static_cast<uint64_t>(src0) * static_cast<uint64_t>(src1);
      if (i.GetModifier()[0] == "hi") {
        regs[i.GetDstRegister()].SetValue(static_cast<uint32_t>(double_size_dst >> 32) + src2);
        regs[i.GetDstRegister()].SetType(kU32);
      } else if (i.GetModifier()[0] == "lo") {
        regs[i.GetDstRegister()].SetValue(static_cast<uint32_t>(double_size_dst & 4294967295) + src2);
        regs[i.GetDstRegister()].SetType(kU32);
      } else if (i.GetModifier()[0] == "wide") {
        regs[i.GetDstRegister()].SetValue(double_size_dst + src2);
        regs[i.GetDstRegister()].SetType(kU64);
      } else {
        Logger::Log("Iss::UB32Iss | Instruction has no valid modifier \"" + i.GetRawLine() + "\"\n", "ERROR");
      }
      break;
    case kMul24: src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB32(regs[i.GetSrcRegister()[1]]);

      double_size_dst = static_cast<uint64_t>(src0) * static_cast<uint64_t>(src1);
      if (i.GetModifier()[0] == "hi") {
        regs[i.GetDstRegister()].SetValue(static_cast<uint32_t>((double_size_dst >> 16) & 4294967295));
        regs[i.GetDstRegister()].SetType(kU32);
      } else if (i.GetModifier()[0] == "lo") {
        regs[i.GetDstRegister()].SetValue(static_cast<uint32_t>(double_size_dst & 4294967295));
        regs[i.GetDstRegister()].SetType(kU32);
      } else {
        Logger::Log("Iss::UB32Iss | Instruction has no valid modifier \"" + i.GetRawLine() + "\"\n", "ERROR");
      }
      break;
    case kMad24: src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB32(regs[i.GetSrcRegister()[1]]);
      src2 = CastWrapperUB32(regs[i.GetSrcRegister()[2]]);

      double_size_dst = static_cast<uint64_t>(src0) * static_cast<uint64_t>(src1);
      if (i.GetModifier()[0] == "hi") {
        regs[i.GetDstRegister()].SetValue(static_cast<uint32_t>((double_size_dst >> 16) & 4294967295) + src2);
        regs[i.GetDstRegister()].SetType(kU32);
      } else if (i.GetModifier()[0] == "lo") {
        regs[i.GetDstRegister()].SetValue(static_cast<uint32_t>(double_size_dst & 4294967295) + src2);
        regs[i.GetDstRegister()].SetType(kU32);
      } else {
        Logger::Log("Iss::UB32Iss | Instruction has no valid modifier \"" + i.GetRawLine() + "\"\n", "ERROR");
      }
      break;
    case kSad: src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB32(regs[i.GetSrcRegister()[1]]);
      src2 = CastWrapperUB32(regs[i.GetSrcRegister()[2]]);

      dst = (src0 < src1 ? src1 - src0 : src0 - src1) + src2;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU32);
      break;
    case kDiv: src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB32(regs[i.GetSrcRegister()[1]]);

      dst = src0 / src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU32);
      break;
    case kRem: src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB32(regs[i.GetSrcRegister()[1]]);

      dst = src0 % src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU32);
      break;
    case kMin: src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB32(regs[i.GetSrcRegister()[1]]);

      dst = src0 < src1 ? src0 : src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU32);
      break;
    case kMax: src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB32(regs[i.GetSrcRegister()[1]]);

      dst = src0 > src1 ? src0 : src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU32);
      break;
    case kBfind:
      //TODO: shiftamt
      src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);

      dst = 4294967295;
      for (int j = 31; j > -1; j--) {
        if ((src0 >> j) & 1) {
          dst = j;
          break;
        }
      }
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU32);
      break;
      // Comparison and Selection Instructions
      // ..
    case kSetp: src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB32(regs[i.GetSrcRegister()[1]]);

      // TODO: Missing
      if (i.GetModifier()[0] == "eq") {
        pred = src0 == src1;
      } else if (i.GetModifier()[0] == "ne") {
        pred = src0 != src1;
      } else if (i.GetModifier()[0] == "lt") {
        pred = src0 < src1;
      } else if (i.GetModifier()[0] == "le") {
        pred = src0 <= src1;
      } else if (i.GetModifier()[0] == "gt") {
        pred = src0 > src1;
      } else if (i.GetModifier()[0] == "ge") {
        pred = src0 >= src1;
      }

      regs[i.GetDstRegister()].SetValue(pred);
      regs[i.GetDstRegister()].SetType(kPred);
      break;
      // Data Movement and Conversion Instructions
      // ...
    case kMov: src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);

      dst = src0;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU32);
      break;
    case kCvt: src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);

      if (i.GetModifier()[i.GetModifier().size() - 2] == "u8") {
        regs[i.GetDstRegister()].SetValue(static_cast<uint8_t>(src0));
        regs[i.GetDstRegister()].SetType(kU8);
      } else if (i.GetModifier()[i.GetModifier().size() - 2] == "u16") {
        regs[i.GetDstRegister()].SetValue(static_cast<uint16_t>(src0));
        regs[i.GetDstRegister()].SetType(kU16);
      } else if (i.GetModifier()[i.GetModifier().size() - 2] == "u32") {
        regs[i.GetDstRegister()].SetValue(static_cast<uint32_t>(src0));
        regs[i.GetDstRegister()].SetType(kU32);
      } else if (i.GetModifier()[i.GetModifier().size() - 2] == "u64") {
        regs[i.GetDstRegister()].SetValue(static_cast<uint64_t>(src0));
        regs[i.GetDstRegister()].SetType(kU64);
      } else if (i.GetModifier()[i.GetModifier().size() - 2] == "s8") {
        regs[i.GetDstRegister()].SetValue(static_cast<int8_t>(src0));
        regs[i.GetDstRegister()].SetType(kS8);
      } else if (i.GetModifier()[i.GetModifier().size() - 2] == "s16") {
        regs[i.GetDstRegister()].SetValue(static_cast<int16_t>(src0));
        regs[i.GetDstRegister()].SetType(kS16);
      } else if (i.GetModifier()[i.GetModifier().size() - 2] == "s32") {
        regs[i.GetDstRegister()].SetValue(static_cast<int32_t>(src0));
        regs[i.GetDstRegister()].SetType(kS32);
      } else if (i.GetModifier()[i.GetModifier().size() - 2] == "s64") {
        regs[i.GetDstRegister()].SetValue(static_cast<int64_t>(src0));
        regs[i.GetDstRegister()].SetType(kS64);
      } else {
        Logger::Log("Iss::UB32Iss | Conversion from u32 to " + i.GetModifier()[i.GetModifier().size() - 2]
                        + " not supported yet\n", "ERROR");
      }
      /*} else if (i.GetModifier()[i.GetModifier().size()-2] == "f16") {
        regs[i.GetDstRegister()].SetValue(static_cast<float>(src0));
      } else if (i.GetModifier()[i.GetModifier().size()-2] == "f32") {
        regs[i.GetDstRegister()].SetValue(static_cast<float>(src0));
      } else if (i.GetModifier()[i.GetModifier().size()-2] == "f64") {
        regs[i.GetDstRegister()].SetValue(static_cast<float>(src0));
      }*/
      break;
      /* --- */
      /*case kMov:
        //TODO: tid.x ...
        regs[i.GetDstRegister()].SetValue(regs[i.GetSrcRegister()[0]].GetValue());
        break;
      case kSetp:
        //TODO: Check which setp (lt, gt, ...)
        regs[i.GetDstRegister()].SetValue(boost::any_cast<uint32_t>(regs[i.GetSrcRegister()[0]].GetValue())
                                              < static_cast<uint32_t>(std::stoi(i.GetSrcRegister()[1])));
        break;
      case kShl:regs[i.GetDstRegister()].SetValue(boost::any_cast<uint32_t>(regs[i.GetSrcRegister()[0]].GetValue())
                                                      << std::stoi(i.GetSrcRegister()[1]));

        break;*/
      // Integer Arithmetic Instructions
    case kPopc: src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);

      dst = 0;
      for (int pos = 31; pos > -1; pos--) {
        if ((src0 >> pos) & 1) {
          ++dst;
        }
      }
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU32);
      break;
    case kClz: src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);

      dst = 0;
      for (int pos = 31; pos > -1 && !(src0 >> pos); pos--) {
        ++dst;
      }
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU32);
      break;
      /*case kFns:
        src0 = boost::any_cast<std::bitset<32>>(regs[i.GetSrcRegister()[0]]);
        i32 = boost::any_cast<int32_t>(regs[i.GetSrcRegister()[2]]);
        switch (regs[i.GetSrcRegister()[1]].GetType()) {
          case kB32:
            src1 = boost::any_cast<std::bitset<32>>(regs[i.GetSrcRegister()[1]].GetValue());
            break;
          case kU32:
            u32 = boost::any_cast<uint32_t>(regs[i.GetSrcRegister()[1]].GetValue());
            break;
          case kS32:
            i32_2 = boost::any_cast<int32_t>(regs[i.GetSrcRegister()[1]].GetValue());
            break;
          default:
            std::cerr << "Wrong register type in register: " << i.GetSrcRegister()[1] << "\n";
            return regs;
        }

        regs[i.GetDstRegister()].SetValue(dst);
        break;*/
    case kBfe:
      //TODO:
      src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB32(regs[i.GetSrcRegister()[1]]);
      src2 = CastWrapperUB32(regs[i.GetSrcRegister()[2]]);

      dst = 0;
      bs0 = 0;
      bs1 = src0;
      for (unsigned int i = 0; i <= 31; i++) {
        bs0[i] = (i < (src2 & 0xff) && (src1 & 0xff) + i <= 31) ? bs1[(src1 & 0xff) + 1] : 0;
      }
      dst = bs0.to_ulong();
      regs[i.GetDstRegister()].SetValue(dst);
      break;
      // Logic and Shift Instructions
    case kAnd: src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB32(regs[i.GetSrcRegister()[1]]);

      dst = src0 & src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU32);
      break;
      // ...
    case kOr: src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB32(regs[i.GetSrcRegister()[1]]);

      dst = src0 | src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU32);
      break;
    case kShl: src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB32(regs[i.GetSrcRegister()[1]]);

      dst = src0 << src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU32);
      break;
    case kShr: src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB32(regs[i.GetSrcRegister()[1]]);

      dst = src0 >> src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU32);
      break;
      // Comparison and Selection Instructions
    case kSelp:src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB32(regs[i.GetSrcRegister()[1]]);
      src2 = CastWrapperUB32(regs[i.GetSrcRegister()[2]]);

      if (src2) {
        dst = src0;
      } else {
        dst = src1;
      }
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU32);
      break;
    case kNot: src0 = CastWrapperUB32(regs[i.GetSrcRegister()[0]]);

      dst = !src0;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU32);
      break;
    default: Logger::Log("Iss::UB32Iss | Instruction not implemented: \"" + i.GetRawLine() + "\"\n", "ERROR");
  }

  return regs;
}

