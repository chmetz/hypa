#ifndef PTX_ANALYSER_SRC_PTX_STRUCTURES_PTX_INSTRUCTION_H_
#define PTX_ANALYSER_SRC_PTX_STRUCTURES_PTX_INSTRUCTION_H_

#include <string>
#include <vector>
#include <iostream>

#include "../data_structures/instruction_keyword.h"
#include "../data_structures/instruction_category.h"
#include "../data_structures/graph.h"
#include "../data_structures/type.h"
#include "../data_structures/parents.h"

class PtxInstruction : public Parents<PtxInstruction> {
  unsigned int id_;
  unsigned int block_id_;
  InstructionKeyword keyword_;
  Type instruction_type_;
  InstructionCategory instruction_category_;
  bool guard_predicate_ = false;
  bool guard_mod_;
  std::string guard_register_;
  std::string raw_line_;
  std::vector<std::string> args_;
  std::set<PtxInstruction> parents_;

  std::vector<std::string> GetArgs(const std::string &line);
  void HandleKeyword();
 public:
  InstructionCategory type_;
  unsigned int bit_width_;
  int weight_ = 1;

  PtxInstruction(unsigned int id, std::string line, std::string name);
  bool operator<(const PtxInstruction &i) const;
  bool operator==(const PtxInstruction &i) const;
  friend std::ostream &operator<<(std::ostream &out, const PtxInstruction &i);
  explicit operator std::string() const;
  const std::string &GetRawLine() const;
  std::string GetDstRegister() const;
  unsigned int GetId() const;
  std::vector<std::string> GetSrcRegister() const;
  std::string GetInstructionName() const;
  InstructionKeyword GetInstructionKeyword() const;
  Type GetInstructionType() const;
  std::vector<std::string> GetModifier() const;
  bool HasGuard() const;
  std::string GetGuardRegister() const;
  bool GetGuardMod() const;
  void SetBlockId(unsigned int block_id);
  unsigned int GetBlockId() const;
  InstructionCategory GetInstructionCategory() const;
  static std::string ExtractReg(const std::string &r);
  void AddParent(const PtxInstruction &i);
  std::set<PtxInstruction> GetParents();
};

#endif //PTX_ANALYSER_SRC_PTX_STRUCTURES_PTX_INSTRUCTION_H_

