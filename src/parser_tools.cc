#include "parser_tools.h"
#include "data_structures/register_state.h"
#include "data_structures/type.h"

#include <boost/regex.hpp>
#include <iostream>

/**
 * \brief Removes spaces and tabs in front of other characters
 * \param s The string which should be trimmed
 * \return the reference to the trimmed string
 */
std::string &Trim(std::string &s) {
  s.erase(0, s.find_first_not_of("\t "));
  return s;
}

std::string &RemoveCommas(std::string &s) {
  for (size_t i = 0; i < s.length(); i++) {
    if (s[i] == ',') {
      s.erase(i, 1);
      i--;
    }
  }

  return s;
}

size_t NextArgWhitespaceSeperated(const std::string &s) {
  size_t pos = s.find_first_of(" \t");

  if (pos != std::string::npos) {
    size_t pos1 = s.substr(pos).find_first_not_of("\t ");
    if (pos1 != std::string::npos) {
      return pos + pos1;
    } else {
      return -1;
    }
  } else {
    return -1;
  }

  return pos;
}

std::string GetArgumentNSeperatedBySpaces(const std::string &s, int n) {
  std::string result;
  size_t currentSpace = 0;

  for (int i = 0; i < n; i++) {
    size_t space = s.find_first_of(" \t", currentSpace);
    if (space == currentSpace) {
      throw std::invalid_argument(
          "probalby no nth argument. Argument number: " + std::to_string(n) + " in line: " + s + "\n");
    }
    size_t argument = s.find_first_not_of(" \t", space);
    size_t end = s.find_first_of(" \t", argument);
    result = s.substr(argument, end - argument);
    currentSpace = ++space;
  }

  return result;
}

Type GetType(const std::string &s) {
  if (s == ".s8") { return kS8; }
  else if (s == ".s16") { return kS16; }
  else if (s == ".s32") { return kS32; }
  else if (s == ".s64") { return kS64; }
  else if (s == ".u8") { return kU8; }
  else if (s == ".u16") { return kU16; }
  else if (s == ".u32") { return kU32; }
  else if (s == ".u64") { return kU64; }
  else if (s == ".f16") { return kF16; }
  else if (s == ".f16x2") { return kF16X2; }
  else if (s == ".f32") { return kF32; }
  else if (s == ".f64") { return kF64; }
  else if (s == ".b8") { return kB8; }
  else if (s == ".b16") { return kB16; }
  else if (s == ".b32") { return kB32; }
  else if (s == ".b64") { return kB64; }
  else if (s == ".pred") { return kPred; }
  else { return kNoType; }
}

/*std::string GetName(const std::string &s) {

}*/

/**
 * \brief Checks if a string starts with "//"
 * \param line The string to be checked
 * \return true if the string starts with "//"
 * TODO: support multiline comments
 */
bool IsComment(const std::string &line) {
  return boost::regex_match(line, boost::regex("//.*"));
}

/**
 * \brief Checks if the string starts with a "."
 * \param line The string to be checked
 * \return true if the string starts with "."
 */
bool IsDirective(const std::string &line) {
  return boost::regex_match(line, boost::regex("\\..*"));
}

// TODO: what if an instruction follows an label
bool IsLabel(const std::string &line) {
  return boost::regex_match(line, boost::regex(".*:.*"));
}

bool HandleDirective(const std::string &line, const std::string &directive, std::string &storage) {
  boost::smatch match;
  if (storage.empty() && boost::regex_search(line, match, boost::regex("\\." + directive + " "))) {
    storage = match.suffix();
    return true;
  } else {
    return false;
  }
}

void handle_line(const std::string &line, PtxProgram &ptx) {
  if (IsDirective(line)) {
    boost::smatch match;
    if (HandleDirective(line, "version", ptx.version_) || HandleDirective(line, "target", ptx.target_)
        || HandleDirective(line, "address_size", ptx.address_size_)) {
      return;
    } else if (boost::regex_search(line, match, boost::regex("\\.reqntid "))) {
      boost::smatch vector_x;
      boost::smatch vector_y;
      boost::smatch vector_z;

      std::string params(match.suffix());

      boost::regex_search(params, vector_x, boost::regex("[0-9]+"));
      boost::regex_search(params, vector_y, boost::regex(", *[0-9] *, *"));
      boost::regex_search(params, vector_z, boost::regex(" *, *[0-9]+"));

      std::string y(vector_y[0]);
      std::string z(vector_z[0]);

      ptx.InitializeNewPart(std::stoi(vector_x[0]), std::stoi(RemoveCommas(y)), std::stoi(RemoveCommas(z)));
    } else if (line.substr(0, 4) == ".reg") {
      ptx.AddRegister(GetArgumentNSeperatedBySpaces(line.substr(0, line.size() - 1), 2),
                      GetType(GetArgumentNSeperatedBySpaces(line, 1)),
                      kRegister);
    } else if (line.substr(0, 5) == ".sreg") {
      ptx.AddRegister(GetArgumentNSeperatedBySpaces(line.substr(0, line.size() - 1), 2),
                      GetType(GetArgumentNSeperatedBySpaces(line, 1)),
                      kSpecialRegister);
    } else if (line.substr(0, 6) == ".const") {
      ptx.AddRegister(GetArgumentNSeperatedBySpaces(line.substr(0, line.size() - 1), 2),
                      GetType(GetArgumentNSeperatedBySpaces(line, 1)),
                      kConstRegister);
    } else if (line.substr(0, 7) == ".global") {
      ptx.AddRegister(GetArgumentNSeperatedBySpaces(line.substr(0, line.size() - 1), 2),
                      GetType(GetArgumentNSeperatedBySpaces(line, 1)),
                      kGlobalMemory);
    } else if (line.substr(0, 6) == ".local") {
      ptx.AddRegister(GetArgumentNSeperatedBySpaces(line.substr(0, line.size() - 1), 2),
                      GetType(GetArgumentNSeperatedBySpaces(line, 1)),
                      kLocalMemory);
    } else if (line.substr(0, 6) == ".param") {
      ptx.AddRegister(GetArgumentNSeperatedBySpaces(line.substr(0, line.size() - 1), 2),
                      GetType(GetArgumentNSeperatedBySpaces(line, 1)),
                      kKernelParameter);
    } else if (line.substr(0, 7) == ".shared") {
      ptx.AddRegister(GetArgumentNSeperatedBySpaces(line.substr(0, line.size() - 1), 2),
                      GetType(GetArgumentNSeperatedBySpaces(line, 1)),
                      kSharedMemory);
    } else if (line.substr(0, 4) == ".tex") {
      ptx.AddRegister(GetArgumentNSeperatedBySpaces(line.substr(0, line.size() - 1), 2),
                      GetType(GetArgumentNSeperatedBySpaces(line, 1)),
                      kTextureMemory);
    }
  } else if (IsLabel(line)) {
    ptx.InitializeNewBlock(line.substr(0, line.size() - 1));
    // TODO: What if { or } is not the only character in the line
  } else if (line == "{") {
    ptx.SetGlobalDefinitions(false);
  } else if (line == "}") {
    ptx.SetGlobalDefinitions(true);
  } else if (line != "{" && line != "}" && line != "(" && line != ")") {
    boost::smatch m;
    boost::regex_search(line, m, boost::regex("[a-zA-Z]+"));

    if (m.size() == 1) {
      PtxInstruction i(ptx.GetNewInstructionId(), line, m[0]);
      ptx.AddInstructionEdge(i);
      ptx.AddRegisterEdge(i);
      ptx.AddInstruction(i);

      if (i.GetInstructionKeyword() == kBra) {
        ptx.AddImportantInstruction(i);
      } else if (i.GetInstructionKeyword() == kRet) {
        ptx.AddImportantInstruction(i);
      }
    } else {
      // std::cout << "Instruction not found: " << line << std::endl;
    }
  }
}

