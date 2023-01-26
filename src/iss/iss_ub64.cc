#include "iss.h"

#include "../logger.h"

std::map<std::string, PtxRegister> &Iss::UB64Iss(const PtxInstruction &i, std::map<std::string, PtxRegister> &regs) {
  uint64_t src0;
  uint64_t src1;
  uint64_t src2;
  uint64_t dst;
  uint32_t dst_32;
  __uint128_t double_size_dst;
  bool pred;

  switch (i.GetInstructionKeyword()) {
    // Integer Arithmetic Instructions
    case kAdd: src0 = CastWrapperUB64(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB64(regs[i.GetSrcRegister()[1]]);

      dst = src0 + src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU64);
      break;
    case kSub: src0 = CastWrapperUB64(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB64(regs[i.GetSrcRegister()[1]]);

      dst = src0 - src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU64);
      break;
    case kMul: src0 = CastWrapperUB64(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB64(regs[i.GetSrcRegister()[1]]);

      double_size_dst = static_cast<__uint128_t>(src0) * static_cast<__uint128_t>(src1);
      if (i.GetModifier()[0] == "hi") {
        regs[i.GetDstRegister()].SetValue(static_cast<uint64_t>(double_size_dst >> 64));
        regs[i.GetDstRegister()].SetType(kU64);
      } else if (i.GetModifier()[0] == "lo") {
        regs[i.GetDstRegister()].SetValue(static_cast<uint64_t>(double_size_dst & 18446744073709551615ULL));
        regs[i.GetDstRegister()].SetType(kU64);
      } else {
        Logger::Log("Iss::UB64Iss | Instruction has no valid modifier \"" + i.GetRawLine() + "\"\n", "ERROR");
      }
      break;
    case kMad: src0 = CastWrapperUB64(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB64(regs[i.GetSrcRegister()[1]]);
      src2 = CastWrapperUB64(regs[i.GetSrcRegister()[2]]);

      double_size_dst = static_cast<__uint128_t>(src0) * static_cast<__uint128_t>(src1);
      if (i.GetModifier()[0] == "hi") {
        regs[i.GetDstRegister()].SetValue(static_cast<uint64_t>(double_size_dst >> 64) + src2);
        regs[i.GetDstRegister()].SetType(kU64);
      } else if (i.GetModifier()[0] == "lo") {
        regs[i.GetDstRegister()].SetValue(static_cast<uint64_t>(double_size_dst & 18446744073709551615ULL) + src2);
        regs[i.GetDstRegister()].SetType(kU64);
      } else {
        Logger::Log("Iss::UB64Iss | Instruction has no valid modifier \"" + i.GetRawLine() + "\"\n", "ERROR");
      }
      break;
    case kSad: src0 = CastWrapperUB64(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB64(regs[i.GetSrcRegister()[1]]);
      src2 = CastWrapperUB64(regs[i.GetSrcRegister()[2]]);

      dst = (src0 < src1 ? src1 - src0 : src0 - src1) + src2;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU64);
      break;
    case kDiv: src0 = CastWrapperUB64(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB64(regs[i.GetSrcRegister()[1]]);

      dst = src0 / src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU64);
      break;
    case kRem: src0 = CastWrapperUB64(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB64(regs[i.GetSrcRegister()[1]]);

      dst = src0 % src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU64);
      break;
    case kMin: src0 = CastWrapperUB64(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB64(regs[i.GetSrcRegister()[1]]);

      dst = src0 < src1 ? src0 : src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU64);
      break;
    case kMax: src0 = CastWrapperUB64(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB64(regs[i.GetSrcRegister()[1]]);

      dst = src0 > src1 ? src0 : src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU64);
      break;
    case kBfind:
      //TODO: shiftamt
      src0 = CastWrapperUB64(regs[i.GetSrcRegister()[0]]);

      dst_32 = 4294967295;
      for (int j = 63; j > -1; j--) {
        if ((src0 >> j) & 1) {
          dst_32 = j;
          break;
        }
      }
      regs[i.GetDstRegister()].SetValue(dst_32);
      regs[i.GetDstRegister()].SetType(kU64);
      break;
      // Data Movement and Conversion Instructions
      // ...
    case kCvt: src0 = CastWrapperUB64(regs[i.GetSrcRegister()[0]]);

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
        Logger::Log("Iss::UB64Iss | Conversion from u64 to " + i.GetModifier()[i.GetModifier().size() - 2]
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
      // Integer Arithmetic Instructions
    case kPopc: src0 = CastWrapperUB64(regs[i.GetSrcRegister()[0]]);

      dst_32 = 0;
      for (int pos = 63; pos > -1; pos--) {
        if ((src0 >> pos) & 1) {
          ++dst_32;
        }
      }
      regs[i.GetDstRegister()].SetValue(dst_32);
      regs[i.GetDstRegister()].SetType(kU32);
      break;
    case kClz: src0 = CastWrapperUB64(regs[i.GetSrcRegister()[0]]);

      dst_32 = 0;
      for (int pos = 63; pos > -1 && !(src0 >> pos); pos--) {
        ++dst_32;
      }
      regs[i.GetDstRegister()].SetValue(dst_32);
      regs[i.GetDstRegister()].SetType(kU32);
      break;
      // Logic and Shift Instructions
    case kAnd: src0 = CastWrapperUB64(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB64(regs[i.GetSrcRegister()[1]]);

      dst = src0 & src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU64);
      break;
      // ...
    case kOr: src0 = CastWrapperUB64(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB64(regs[i.GetSrcRegister()[1]]);

      dst = src0 | src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU64);
      break;
    case kShl: src0 = CastWrapperUB64(regs[i.GetSrcRegister()[0]]);
      dst_32 = CastWrapperUB32(regs[i.GetSrcRegister()[1]]);

      dst = src0 << dst_32;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU64);
      break;
    case kShr: src0 = CastWrapperUB64(regs[i.GetSrcRegister()[0]]);
      dst_32 = CastWrapperUB32(regs[i.GetSrcRegister()[1]]);

      dst = src0 >> dst_32;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU64);
      break;
      // Comparison and Selection Instructions
    case kSelp:src0 = CastWrapperUB64(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB64(regs[i.GetSrcRegister()[1]]);
      src2 = CastWrapperUB64(regs[i.GetSrcRegister()[2]]);

      if (src2) {
        dst = src0;
      } else {
        dst = src1;
      }
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kU64);
      break;
    case kSetp: src0 = CastWrapperUB64(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperUB64(regs[i.GetSrcRegister()[1]]);

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
    default: Logger::Log("Iss::UB64Iss | Instruction not implemented: \"" + i.GetRawLine() + "\"\n", "ERROR");
  }

  return regs;
}

