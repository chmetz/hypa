#ifndef PTX_ANALYSER_SRC_ISS_ISS_H_
#define PTX_ANALYSER_SRC_ISS_ISS_H_

#include <set>
#include <list>

#include "../ptx_structures/ptx_program.h"
#include "../data_structures/ptx_register.h"

class Iss {
  static std::map<std::string, PtxRegister> &U8Iss(const PtxInstruction &i, std::map<std::string, PtxRegister> &regs);
  static std::map<std::string, PtxRegister> &UB16Iss(const PtxInstruction &i, std::map<std::string, PtxRegister> &regs);
  static std::map<std::string, PtxRegister> &UB32Iss(const PtxInstruction &i, std::map<std::string, PtxRegister> &regs);
  static std::map<std::string, PtxRegister> &UB64Iss(const PtxInstruction &i, std::map<std::string, PtxRegister> &regs);
  static std::map<std::string, PtxRegister> &S8Iss(const PtxInstruction &i, std::map<std::string, PtxRegister> &regs);
  static std::map<std::string, PtxRegister> &S16Iss(const PtxInstruction &i, std::map<std::string, PtxRegister> &regs);
  static std::map<std::string, PtxRegister> &S32Iss(const PtxInstruction &i, std::map<std::string, PtxRegister> &regs);
  static std::map<std::string, PtxRegister> &S64Iss(const PtxInstruction &i, std::map<std::string, PtxRegister> &regs);
  static std::map<std::string, PtxRegister> &PredIss(const PtxInstruction &i, std::map<std::string, PtxRegister> &regs);
  static std::map<std::string, PtxRegister> &F16Iss(const PtxInstruction &i, std::map<std::string, PtxRegister> &regs);
  static std::map<std::string, PtxRegister> &F32Iss(const PtxInstruction &i, std::map<std::string, PtxRegister> &regs);
  static std::map<std::string, PtxRegister> &F64Iss(const PtxInstruction &i, std::map<std::string, PtxRegister> &regs);

  static int8_t CastWrapperS8(const PtxRegister &reg);
  static int16_t CastWrapperS16(const PtxRegister &reg);
  static int32_t CastWrapperS32(const PtxRegister &reg);
  static int64_t CastWrapperS64(const PtxRegister &reg);
  static uint8_t CastWrapperU8(const PtxRegister &reg);
  static uint16_t CastWrapperUB16(const PtxRegister &reg);
  static uint32_t CastWrapperUB32(const PtxRegister &reg);
  static uint64_t CastWrapperUB64(const PtxRegister &reg);
  static bool CastWrapperPred(const PtxRegister &reg);
 public:
  static std::map<std::string, PtxRegister> &Execute(const std::set<PtxInstruction> &instructions,
                                                     std::map<std::string, PtxRegister> &regs);
};

#endif //PTX_ANALYSER_SRC_ISS_ISS_H_

