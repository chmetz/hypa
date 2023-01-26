#include "iss.h"

#include "../logger.h"

std::map<std::string, PtxRegister> &Iss::PredIss(const PtxInstruction &i, std::map<std::string, PtxRegister> &regs) {
  bool src0;
  bool src1;
  bool dst;

  switch (i.GetInstructionKeyword()) {
    // Logic and Shift Instructions
    case kAnd: src0 = CastWrapperPred(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperPred(regs[i.GetSrcRegister()[1]]);

      dst = src0 & src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kPred);
      break;
      // ...
    case kOr: src0 = CastWrapperPred(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperPred(regs[i.GetSrcRegister()[1]]);

      dst = src0 | src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kPred);
      break;
    default: Logger::Log("Iss::PredIss | Instruction not implemented: \"" + i.GetRawLine() + "\"\n", "ERROR");
  }

  return regs;
}

