#ifndef PTX_ANALYSER_SRC_PTX_STRUCTURES_PTX_PART_H_
#define PTX_ANALYSER_SRC_PTX_STRUCTURES_PTX_PART_H_

#include <list>
#include <map>

#include "ptx_instruction.h"
#include "../data_structures/type.h"
#include "../data_structures/register_state.h"
#include "../data_structures/graph.h"
#include "../data_structures/instruction_counts.h"

class PtxPart {
  uint32_t ctaid_x;
  unsigned int currentBlock_ = 0;
  Graph<std::string> registerGraph_;
  Graph<PtxInstruction> instructionGraph_;
  std::pair<InstructionCounts, std::list<unsigned int>> ExecuteOneThread(unsigned int tid);
 public:
  unsigned int threads_;
  std::map<std::pair<unsigned int, std::string>, std::list<PtxInstruction>> blocks_;
  std::map<std::string, RegisterState> regState_;
  std::map<std::string, Type> regType_;
  std::map<std::string, unsigned int> regUses_;

  PtxPart(unsigned int id, unsigned int x, unsigned int y, unsigned int z);
  void InitializeNewBlock(const std::string &name);
  void AddInstruction(PtxInstruction &i);
  void AddRegister(const std::string &name, Type type, RegisterState state);
  unsigned int GetNumberRegisters(const RegisterState &rs) const;
  void ExportRegisterGraphs(std::string filename);
  void ExportInstructionGraphs(std::string filename);
  InstructionCounts Execute();
  size_t GetNumberOfInstructionsToExecute() const;
  void AddInstructionEdge(PtxInstruction &i);
  void AddRegisterEdge(const PtxInstruction &i);
  void AddImportantInstruction(const PtxInstruction &i);
};

#endif //PTX_ANALYSER_SRC_PTX_STRUCTURES_PTX_PART_H_

