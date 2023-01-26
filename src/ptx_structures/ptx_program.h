#ifndef PTX_ANALYSER_SRC_PTX_STRUCTURES_PTX_PROGRAM_H_
#define PTX_ANALYSER_SRC_PTX_STRUCTURES_PTX_PROGRAM_H_

#include <string>
#include <vector>
#include <map>

#include "ptx_part.h"
#include "../data_structures/register_state.h"
#include "../data_structures/type.h"
#include "../data_structures/graph.h"

class PtxProgram {
  unsigned int instruction_id_counter_ = 0;
  bool global_definitions_ = true;
 public:
  std::string version_;
  std::string target_;
  std::string address_size_;
  unsigned int instructions_ = 0;
  unsigned int weighted_instructions_ = 0;
  unsigned int threads_ = 0;
  std::vector<PtxPart> parts_;
  std::map<std::string, RegisterState> regState_;
  std::map<std::string, Type> regType_;

  void AddInstruction(PtxInstruction &i);
  void InitializeNewPart(unsigned int x, unsigned int y, unsigned int z);
  void InitializeNewBlock(const std::string &name);
  unsigned int GetInstructionsAllThreads();
  unsigned int GetTotalFPI();
  unsigned int GetCTACount() const;
  void AddRegister(const std::string &name, Type type, RegisterState state);
  unsigned int GetNumberRegisters(const RegisterState &rs);
  void ExportGraphs(std::string folder);
  void SetGlobalDefinitions(bool state);
  unsigned int GetNewInstructionId();
  void AddInstructionEdge(PtxInstruction &i);
  void AddRegisterEdge(const PtxInstruction &i);
  void AddImportantInstruction(const PtxInstruction &i);
  size_t GetNumberOfInstructionsToExecute() const;
};

#endif //PTX_ANALYSER_SRC_PTX_STRUCTURES_PTX_PROGRAM_H_

