#include "ptx_part.h"
#include "../data_structures/ptx_register.h"
#include "../iss/iss.h"
#include "../data_structures/options.h"
#include "../logger.h"
#include "../data_structures/string_helpers.h"

#include <ranges>
#include <boost/regex.hpp>
#include <iostream>

PtxPart::PtxPart(unsigned int id, unsigned int x, unsigned int y, unsigned int z) {
  threads_ = x * y * z;
  ctaid_x = id;
}

void PtxPart::AddInstruction(PtxInstruction &i) {
  i.SetBlockId(currentBlock_);
  if (blocks_.empty()) {
    std::pair<unsigned int, std::string> p(0, "");
    blocks_.insert({p, std::list<PtxInstruction>({i})});
  } else {
    blocks_.rbegin()->second.push_back(i);
  }

  boost::sregex_token_iterator sregx_it(i.GetRawLine().begin(), i.GetRawLine().end(), boost::regex("%[a-zA-Z0-9]*"), 0);
  boost::sregex_token_iterator end;

  for (; sregx_it != end; ++sregx_it) {
    std::map<std::string, unsigned int>::iterator mit;
    std::string name(*sregx_it);

    mit = regUses_.find(name.substr(1));
    if (mit != regUses_.end()) {
      ++regUses_[name.substr(1)];
    }
  }
}

void PtxPart::InitializeNewBlock(const std::string &name) {
  std::pair<unsigned int, std::string> p(++currentBlock_, name);
  blocks_.insert({p, std::list<PtxInstruction>()});
}

void PtxPart::AddRegister(const std::string &name, Type type, RegisterState state) {
  boost::smatch mname;
  boost::smatch m;
  if (boost::regex_search(name, mname, boost::regex("[a-z_0-9A-Z]+"))) {
  }
  if (boost::regex_search(name, m, boost::regex("<[0-9]+"))) {
    int range = std::stoi(m[0].str().substr(1, m[0].str().length()));
    for (int i = 0; i < range; i++) {
      regState_["%" + mname[0] + std::to_string(i)] = state;
      regType_["%" + mname[0] + std::to_string(i)] = type;
      regUses_["%" + mname[0] + std::to_string(i)] = 0;
    }
  } else {
    regState_["%" + mname[0]] = state;
    regType_["%" + mname[0]] = type;
    regUses_["%" + mname[0]] = 0;
  }
}

unsigned int PtxPart::GetNumberRegisters(const RegisterState &rs) const {
  unsigned int count = 0;

  for (const auto &[key, value] : regUses_) {
    if (regState_.at(key) == rs) {
      count += value;
    }
  }

  return count;
}

void PtxPart::ExportRegisterGraphs(std::string filename) {
  registerGraph_.ExportDotFile(filename);
}

void PtxPart::ExportInstructionGraphs(std::string filename) {
  instructionGraph_.ExportDotFile(filename);
}

InstructionCounts PtxPart::Execute() {
  InstructionCounts result;
  if (Options::one_thread) {
    auto [c, et] = ExecuteOneThread(0);
    result.total_ic += (c.total_ic * threads_);
    result.fp_ic += (c.fp_ic * threads_);
    result.exec_ic += c.exec_ic;
  } else {
    //result.db_list = std::list<unsigned int>();
    std::set<std::list<unsigned int>> branch_orders;
    for (unsigned int tid = 0; tid < threads_; tid++) {
      auto [c, et] = ExecuteOneThread(tid);
      branch_orders.insert(et);
      result.total_ic += c.total_ic;
      result.fp_ic += c.fp_ic;
      result.exec_ic += c.exec_ic;
    }
    result.db_list.push_back(branch_orders.size());
    result.divergent_branches = branch_orders.size();
  }

  return result;
}

std::pair<InstructionCounts, std::list<unsigned int>> PtxPart::ExecuteOneThread(unsigned int tid) {
  if (Options::verbose) {
    Logger::Log("Starting Thread: " + std::to_string(tid) + "\n", "DEBUG");
  }

  std::map<std::string, PtxRegister> regs;
  std::set<PtxInstruction> ins = instructionGraph_.GetAncestorsOfImportant();

  std::set<PtxInstruction> allInsPart;
  for (const auto &[id, block] : blocks_) {
    for (const auto &i : block) {
      allInsPart.insert(i);
    }
  }

  /*std::set<PtxInstruction> insPoint = instructionGraph_.GetAncestorsOfImportantFast(allInsPart);
  std::cout << "----- Part " << ctaid_x << " : Thread " << tid << " -----\n";
  std::cout << "Set: ";
  for (auto &i : ins) {
    std::cout << "," << i.GetId();
  }
  std::cout << "\n";
  std::cout << "SetFast: ";
  for (auto &i : insPoint) {
    std::cout << "," << i.GetId();
  }
  std::cout << "\n";*/

  auto importantRegs = registerGraph_.GetImportant();
  auto requiredRegs = registerGraph_.GetAncestorsOfImportant();
  requiredRegs.insert(importantRegs.begin(), importantRegs.end());

  for (const auto &p : requiredRegs) {
    switch (regType_[p]) {
      case kU8: regs[p] = PtxRegister(regType_[p], static_cast<uint8_t>(0));
        break;
      case kU16: regs[p] = PtxRegister(regType_[p], static_cast<uint16_t>(0));
        break;
      case kU32: regs[p] = PtxRegister(regType_[p], static_cast<uint32_t>(0));
        break;
      case kU64: regs[p] = PtxRegister(regType_[p], static_cast<uint64_t>(0));
        break;
      case kS8: regs[p] = PtxRegister(regType_[p], static_cast<int8_t>(0));
        break;
      case kS16: regs[p] = PtxRegister(regType_[p], static_cast<int16_t>(0));
        break;
      case kS32: regs[p] = PtxRegister(regType_[p], static_cast<int32_t>(0));
        break;
      case kS64: regs[p] = PtxRegister(regType_[p], static_cast<int64_t>(0));
        break;
      case kB8: regs[p] = PtxRegister(regType_[p], static_cast<uint8_t>(0));
        break;
      case kB16: regs[p] = PtxRegister(regType_[p], static_cast<uint16_t>(0));
        break;
      case kB32: regs[p] = PtxRegister(regType_[p], static_cast<uint32_t>(0));
        break;
      case kB64: regs[p] = PtxRegister(regType_[p], static_cast<uint64_t>(0));
        break;
      case kPred: regs[p] = PtxRegister(regType_[p], false);
        break;
      default:
        Logger::Log(
            "PtxPart::ExecuteOneThread | No default value for this datatype: " + TypeToString[regType_[p]] + "\n",
            "ERROR");
    }
  }

  regs["%ctaid.x"] = PtxRegister(kU32, ctaid_x);
  regs["%tid.x"] = PtxRegister(kU32, static_cast<uint32_t>(tid));

  // Initialize runtime and tracking vars
  unsigned int block_pointer = 0;
  bool pointer_changed = true;
  std::set<PtxInstruction> working_list;
  std::list<unsigned int> execution_tracker;
  InstructionCounts result;

  while (pointer_changed && block_pointer < blocks_.size()) {
    // reset runtime vars for execution
    pointer_changed = false;
    working_list.clear();

    // load current block into working_list
    for (const auto &i : ins) {
      if (i.GetBlockId() == block_pointer) {
        working_list.insert(i);
      } else if (i.GetBlockId() > block_pointer) {
        break;
      }
    }

    // execute working_list
    Iss::Execute(working_list, regs);
    execution_tracker.push_back(block_pointer);
    result.exec_ic += working_list.size();

    bool has_important = false;
    for (const auto &i : instructionGraph_.GetImportant()) {
      if (i.GetBlockId() == block_pointer) {
        has_important = true;
        break;
      } else if (i.GetBlockId() > block_pointer) {
        break;
      }
    }

    if (!has_important) {
      pointer_changed = true;
      ++block_pointer;
      continue;
    }

    for (const auto &i : instructionGraph_.GetImportant()) {
      if (pointer_changed || i.GetBlockId() > block_pointer) {
        break;
      } else if (i.GetBlockId() < block_pointer) {
        continue;
      }
      if (i.GetInstructionKeyword() == kRet) {
        // terminate the while loop
        block_pointer = blocks_.size();
        break;
      } else if (i.HasGuard()) {
        if (boost::any_cast<bool>(regs[i.GetGuardRegister()].GetValue())) {
          for (const auto &p : blocks_) {
            if (p.first.second != "" && p.first.second == i.GetDstRegister()) {
              if (Options::verbose) {
                Logger::Log("PtxPart::ExecuteOneThread | Setting Jump to " + p.first.second + " (Block number: "
                                + std::to_string(p.first.first) + ")\n", "DEBUG");
              }
              block_pointer = p.first.first;
              pointer_changed = true;
              break;
            }
          }
        }
      } else {
        for (const auto &p : blocks_) {
          if (p.first.second != "" && p.first.second == i.GetDstRegister()) {
            if (Options::verbose) {
              Logger::Log("PtxPart::ExecuteOneThread | Setting Jump to " + p.first.second + " (Block number: "
                              + std::to_string(p.first.first) + ")\n", "DEBUG");
            }
            block_pointer = p.first.first;
            pointer_changed = true;
            break;
          }
        }
      }
    }
  }

  for (const auto &b : execution_tracker) {
    for (const auto &p : blocks_) {
      if (p.first.first == b) {
        result.total_ic += p.second.size();
        for (const auto &i : p.second) {
          if (i.GetInstructionCategory() == kFloatingPoint) {
            ++result.fp_ic;
          }
        }
      }
    }
  }

  return {result, execution_tracker};
}

void PtxPart::AddInstructionEdge(PtxInstruction &i) {
  std::vector<std::string> src = i.GetSrcRegister();

  for (const auto &s : src) {
    bool breakLoop = false;
    for (auto &block : std::ranges::reverse_view(blocks_)) {
      for (auto instrIt = block.second.rbegin(); instrIt != block.second.rend(); ++instrIt) {
        if (s == instrIt->GetDstRegister()) {
          i.AddParent(*instrIt);
          instructionGraph_.AddEdge(*instrIt, i);
        }
      }
    }
  }
}

void PtxPart::AddRegisterEdge(const PtxInstruction &i) {
  if (i.GetInstructionKeyword() != kBra && i.GetInstructionKeyword() != kRet) {
    for (const auto &src : i.GetSrcRegister()) {
      registerGraph_.AddEdge(PtxInstruction::ExtractReg(src), PtxInstruction::ExtractReg(i.GetDstRegister()));
    }
  }
}

void PtxPart::AddImportantInstruction(const PtxInstruction &i) {
  instructionGraph_.AddImportant(i);
  if (i.HasGuard() && !i.GetGuardRegister().empty()) {
    registerGraph_.AddImportant(PtxInstruction::ExtractReg(i.GetGuardRegister()));
  } else if (i.GetInstructionKeyword() != kBra && i.GetInstructionKeyword() != kRet) {
    registerGraph_.AddImportant(PtxInstruction::ExtractReg(i.GetDstRegister()));
  }
}

size_t PtxPart::GetNumberOfInstructionsToExecute() const {
  return instructionGraph_.GetAncestorsOfImportant().size();
}

