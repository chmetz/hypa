#include "iss.h"

#include "../logger.h"

std::map<std::string, PtxRegister> &Iss::U8Iss(const PtxInstruction &i, std::map<std::string, PtxRegister> &regs) {
  uint8_t src0;
  /*uint8_t src1;
  uint8_t dst;*/

  switch (i.GetInstructionKeyword()) {
    // Data Movement and Conversion Instructions
    // ...
    case kCvt: src0 = CastWrapperU8(regs[i.GetSrcRegister()[0]]);

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
        Logger::Log("Iss::U8Iss | Conversion from u8 to " + i.GetModifier()[i.GetModifier().size() - 2]
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
    default: Logger::Log("Iss::U8Iss | Instruction not implemented: \"" + i.GetRawLine() + "\"\n", "ERROR");
  }

  return regs;
}

