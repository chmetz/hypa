#ifndef PTX_ANALYSER_SRC_DATA_STRUCTURES_REGISTER_STATE_H_
#define PTX_ANALYSER_SRC_DATA_STRUCTURES_REGISTER_STATE_H_

enum RegisterState {
  kRegister,
  kSpecialRegister,
  kConstRegister,
  kGlobalMemory,
  kLocalMemory,
  kKernelParameter,
  kSharedMemory,
  kTextureMemory,
};

#endif //PTX_ANALYSER_SRC_DATA_STRUCTURES_REGISTER_STATE_H_

