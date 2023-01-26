#include "ptx_program.h"

#include <iostream>
#include <boost/regex.hpp>

void PtxProgram::AddInstruction(PtxInstruction &i) {
  ++instructions_;
  if (parts_.empty()) {
    PtxPart p = PtxPart(0, 1, 1, 1);
    parts_.push_back(p);
  }
  parts_[parts_.size() - 1].AddInstruction(i);
  weighted_instructions_ += i.weight_;
}

void PtxProgram::InitializeNewPart(unsigned int x, unsigned int y, unsigned int z) {
  threads_ += x * y * z;
  PtxPart p = PtxPart(parts_.size(), x, y, z);
  parts_.push_back(p);
}

void PtxProgram::InitializeNewBlock(const std::string &name) {
  parts_[parts_.size() - 1].InitializeNewBlock(name);
}

unsigned int PtxProgram::GetInstructionsAllThreads() {
  unsigned int count = 0;

  for (const PtxPart &e : parts_) {
    for (const auto &[key, value] : e.blocks_) {
      count += (value.size() * e.threads_);
    }
  }

  return count;
}

unsigned int PtxProgram::GetTotalFPI() {
  unsigned int count = 0;

  for (const PtxPart &e : parts_) {
    for (const auto &[key, value] : e.blocks_) {
      for (const PtxInstruction &i : value) {
        if (i.type_ == kFloatingPoint) {
          count += e.threads_;
        }
      }
    }
  }

  return count;
}

unsigned int PtxProgram::GetCTACount() const {
  return parts_.size();
}

void PtxProgram::AddRegister(const std::string &name, Type type, RegisterState state) {
  if (global_definitions_) {
    boost::smatch mname;
    boost::smatch m;
    if (boost::regex_search(name, mname, boost::regex("[a-z_0-9A-Z]+"))) {
    }
    if (boost::regex_search(name, m, boost::regex("<[0-9]+"))) {
      int range = std::stoi(m[0].str().substr(1, m[0].str().length()));
      for (int i = 0; i < range; i++) {
        regState_[mname[0] + std::to_string(i)] = state;
        regType_[mname[0] + std::to_string(i)] = type;
      }
    } else {
      regState_[mname[0]] = state;
      regType_[mname[0]] = type;
    }
  } else {
    if (parts_.empty()) {
      PtxPart p = PtxPart(0, 1, 1, 1);
      parts_.push_back(p);
    }
    parts_[parts_.size() - 1].AddRegister(name, type, state);
  }
}

unsigned int PtxProgram::GetNumberRegisters(const RegisterState &rs) {
  unsigned int count = 0;

  for (const PtxPart &p : parts_) {
    count += p.GetNumberRegisters(rs);
  }

  return count;
}

void PtxProgram::ExportGraphs(std::string folder) {
  for (size_t i = 0; i < parts_.size(); i++) {
    parts_[i].ExportRegisterGraphs(folder + "/register/Part_" + std::to_string(i) + ".dot");
    parts_[i].ExportInstructionGraphs(folder + "/instructions/Part_" + std::to_string(i) + ".dot");
  }
}

void PtxProgram::SetGlobalDefinitions(bool state) {
  global_definitions_ = state;
}

unsigned int PtxProgram::GetNewInstructionId() {
  return instruction_id_counter_++;
}

void PtxProgram::AddInstructionEdge(PtxInstruction &i) {
  parts_[parts_.size() - 1].AddInstructionEdge(i);
}

void PtxProgram::AddRegisterEdge(const PtxInstruction &i) {
  parts_[parts_.size() - 1].AddRegisterEdge(i);
}

void PtxProgram::AddImportantInstruction(const PtxInstruction &i) {
  parts_[parts_.size() - 1].AddImportantInstruction(i);
}

size_t PtxProgram::GetNumberOfInstructionsToExecute() const {
  size_t count = 0;

  for (const auto &p : parts_) {
    count += p.GetNumberOfInstructionsToExecute();
  }

  return count;
}

