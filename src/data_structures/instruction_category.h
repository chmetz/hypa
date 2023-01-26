#ifndef PTX_ANALYSER_SRC_DATA_STRUCTURES_INSTRUCTION_CATEGORY_H_
#define PTX_ANALYSER_SRC_DATA_STRUCTURES_INSTRUCTION_CATEGORY_H_

// EP = Extended Precision
// HP = Half Precision
enum InstructionCategory {
  kIntegerArithmetic,
  kEPIntegerArithmetic,
  kFloatingPoint,
  kHPFloatingPoint,
  kComparisonSelection,
  kHPComparisonSelection,
  kLogicShift,
  kDataMovementConversion,
  kTexture,
  kSurface,
  kControlFLow,
  kParallelSynchronizationCommunication,
  kWarpLevelMatrixMultiplyAccumulate,
  kVideo,
  kScalarVideo,
  SIMDVideo,
  kMiscellaneous,
};

#endif //PTX_ANALYSER_SRC_DATA_STRUCTURES_INSTRUCTION_CATEGORY_H_

