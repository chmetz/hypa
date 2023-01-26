#include "ptx_instruction.h"

#include "../parser_tools.h"
#include "../data_structures/options.h"

#include <utility>
#include <boost/regex.hpp>
#include <iostream>

std::vector<std::string> PtxInstruction::GetArgs(const std::string &line) {
  std::vector<std::string> v;
  std::string mut_line;

  // Remove Labels in front of the instruction
  size_t pos = line.find(':');
  if (pos != std::string::npos) {
    mut_line = line.substr(pos + 1);
    Trim(mut_line);
  } else {
    mut_line = line;
  }

  if (mut_line[0] == '@') {
    guard_predicate_ = true;
    size_t gpos = mut_line.find_first_of(" \t");
    if (mut_line[1] == '!') {
      guard_register_ = mut_line.substr(2, gpos - 2);
      guard_mod_ = false;
    } else {
      guard_register_ = mut_line.substr(1, gpos - 1);
      guard_mod_ = true;
    }

    mut_line = mut_line.substr(gpos);
    Trim(mut_line);
  }

  // Remove whitespaces and other seperators
  boost::sregex_token_iterator i(mut_line.begin(), mut_line.end(), boost::regex("[^\t ,;]+"), 0);
  boost::sregex_token_iterator end;

  for (; i != end; ++i) {
    v.push_back(*i);
  }

  if (guard_predicate_) {
    v.push_back(guard_register_);
  }

  return v;
}

void PtxInstruction::HandleKeyword() {
  std::string first_arg(args_[0]);

  // get instruction keyword
  std::string keyword;
  size_t keyword_pos = first_arg.find('.');
  if (keyword_pos != std::string::npos) {
    keyword = first_arg.substr(0, keyword_pos);
  } else {
    keyword = first_arg;
  }

  // get data type
  size_t start_data_type_pos = first_arg.find_last_of('.');
  if (start_data_type_pos == std::string::npos) {
    instruction_type_ = kNoType;
  } else {
    size_t data_type_pos = first_arg.find_first_of(" \t", start_data_type_pos);
    std::string data_type = first_arg.substr(start_data_type_pos, data_type_pos - start_data_type_pos);
    instruction_type_ = GetType(data_type);
  }

  if (keyword == "abs") {
    keyword_ = kAbs;
  } else if (keyword == "add") {
    keyword_ = kAdd;
  } else if (keyword == "addc") {
    keyword_ = kAddc;
  } else if (keyword == "and") {
    keyword_ = kAnd;
  } else if (keyword == "atom") {
    keyword_ = kAtom;
  } else if (keyword == "bar") {
    keyword_ = kBar;
  } else if (keyword == "bfe") {
    keyword_ = kBfe;
  } else if (keyword == "bfi") {
    keyword_ = kBfi;
  } else if (keyword == "bfind") {
    keyword_ = kBfind;
  } else if (keyword == "bra") {
    keyword_ = kBra;
  } else if (keyword == "brev") {
    keyword_ = kBrev;
  } else if (keyword == "brkpt") {
    keyword_ = kBrkpt;
  } else if (keyword == "call") {
    keyword_ = kCall;
  } else if (keyword == "clz") {
    keyword_ = kClz;
  } else if (keyword == "cnot") {
    keyword_ = kCnot;
  } else if (keyword == "copysign") {
    keyword_ = kCopysign;
  } else if (keyword == "cos") {
    keyword_ = kCos;
  } else if (keyword == "cvt") {
    keyword_ = kCvt;
  } else if (keyword == "cvta") {
    keyword_ = kCvta;
  } else if (keyword == "div") {
    keyword_ = kDiv;
  } else if (keyword == "ex2") {
    keyword_ = kEx2;
  } else if (keyword == "exit") {
    keyword_ = kExit;
  } else if (keyword == "fma") {
    keyword_ = kFma;
  } else if (keyword == "fns") {
    keyword_ = kFns;
  } else if (keyword == "isspacep") {
    keyword_ = kIsspacep;
  } else if (keyword == "ld") {
    keyword_ = kLd;
  } else if (keyword == "ldu") {
    keyword_ = kLdu;
  } else if (keyword == "lg2") {
    keyword_ = kLg2;
  } else if (keyword == "mad") {
    keyword_ = kMad;
  } else if (keyword == "mad24") {
    keyword_ = kMad24;
  } else if (keyword == "madc") {
    keyword_ = kMadc;
  } else if (keyword == "max") {
    keyword_ = kMax;
  } else if (keyword == "membar") {
    keyword_ = kMembar;
  } else if (keyword == "min") {
    keyword_ = kMin;
  } else if (keyword == "mov") {
    keyword_ = kMov;
  } else if (keyword == "mul") {
    keyword_ = kMul;
  } else if (keyword == "mul24") {
    keyword_ = kMul24;
  } else if (keyword == "neg") {
    keyword_ = kNeg;
  } else if (keyword == "not") {
    keyword_ = kNot;
  } else if (keyword == "or") {
    keyword_ = kOr;
  } else if (keyword == "pmevent") {
    keyword_ = kPmevent;
  } else if (keyword == "popc") {
    keyword_ = kPopc;
  } else if (keyword == "prefetch") {
    keyword_ = kPrefetch;
  } else if (keyword == "prefetchu") {
    keyword_ = kPrefetchu;
  } else if (keyword == "prmt") {
    keyword_ = kPrmt;
  } else if (keyword == "rcp") {
    keyword_ = kRcp;
  } else if (keyword == "red") {
    keyword_ = kRed;
  } else if (keyword == "rem") {
    keyword_ = kRem;
  } else if (keyword == "ret") {
    keyword_ = kRet;
  } else if (keyword == "rsqrt") {
    keyword_ = kRsqrt;
  } else if (keyword == "sad") {
    keyword_ = kSad;
  } else if (keyword == "selp") {
    keyword_ = kSelp;
  } else if (keyword == "set") {
    keyword_ = kSet;
  } else if (keyword == "setp") {
    keyword_ = kSetp;
  } else if (keyword == "shf") {
    keyword_ = kShf;
  } else if (keyword == "shfl") {
    keyword_ = kShfl;
  } else if (keyword == "shl") {
    keyword_ = kShl;
  } else if (keyword == "shr") {
    keyword_ = kShr;
  } else if (keyword == "sin") {
    keyword_ = kSin;
  } else if (keyword == "slct") {
    keyword_ = kSlct;
  } else if (keyword == "sqrt") {
    keyword_ = kSqrt;
  } else if (keyword == "st") {
    keyword_ = kSt;
  } else if (keyword == "sub") {
    keyword_ = kSub;
  } else if (keyword == "subc") {
    keyword_ = kSubc;
  } else if (keyword == "suld") {
    keyword_ = kSuld;
  } else if (keyword == "suq") {
    keyword_ = kSuq;
  } else if (keyword == "sured") {
    keyword_ = kSured;
  } else if (keyword == "sust") {
    keyword_ = kSust;
  } else if (keyword == "testp") {
    keyword_ = kTestp;
  } else if (keyword == "tex") {
    keyword_ = kTex;
  } else if (keyword == "tld4") {
    keyword_ = kTld4;
  } else if (keyword == "trap") {
    keyword_ = kTrap;
  } else if (keyword == "txq") {
    keyword_ = kTxq;
  } else if (keyword == "vabsdiff2") {
    keyword_ = kVabsdiff2;
  } else if (keyword == "vabsdiff4") {
    keyword_ = kVabsdiff4;
  } else if (keyword == "vadd") {
    keyword_ = kVadd;
  } else if (keyword == "vadd2") {
    keyword_ = kVadd2;
  } else if (keyword == "vadd4") {
    keyword_ = kVadd4;
  } else if (keyword == "vavrg2") {
    keyword_ = kVavrg2;
  } else if (keyword == "vavrg4") {
    keyword_ = kVavrg4;
  } else if (keyword == "vmad") {
    keyword_ = kVmad;
  } else if (keyword == "vmax") {
    keyword_ = kVmax;
  } else if (keyword == "vmax2") {
    keyword_ = kVmax2;
  } else if (keyword == "vmax4") {
    keyword_ = kVmax4;
  } else if (keyword == "vmin") {
    keyword_ = kVmin;
  } else if (keyword == "vmin2") {
    keyword_ = kVmin2;
  } else if (keyword == "vmin4") {
    keyword_ = kVmin4;
  } else if (keyword == "vote") {
    keyword_ = kVote;
  } else if (keyword == "vset") {
    keyword_ = kVset;
  } else if (keyword == "vset2") {
    keyword_ = kVset2;
  } else if (keyword == "vset4") {
    keyword_ = kVset4;
  } else if (keyword == "vshl") {
    keyword_ = kVshl;
  } else if (keyword == "vshr") {
    keyword_ = kVshr;
  } else if (keyword == "vsub") {
    keyword_ = kVsub;
  } else if (keyword == "vsub2") {
    keyword_ = kVsub2;
  } else if (keyword == "vsub4") {
    keyword_ = kVsub4;
  } else if (keyword == "xor") {
    keyword_ = kXor;
  }
}

std::string PtxInstruction::ExtractReg(const std::string &r) {
  if (r.front() == '[') {
    size_t pos = r.find('+');
    if (pos == std::string::npos) {
      return r.substr(1, r.length() - 2);
    } else {
      return r.substr(1, pos - 1);
    }
  } else {
    return r;
  }
}

PtxInstruction::PtxInstruction(unsigned int id, std::string line, std::string name) {
  id_ = id;
  // type
  boost::smatch type;
  if (boost::regex_search(line, type, boost::regex(".[a-z][0-9][0-9] "))) {
    if (type[0].str().substr(1, 3) == "f32") {
      bit_width_ = 32;
      if (name == "add" || name == "sub" || name == "mul" || name == "mad" || name == "fma" || name == "div"
          || name == "rcp" || name == "sqrt" || name == "abs" || name == "neg" || name == "min" || name == "max"
          || name == "rsqrt" || name == "sin" || name == "cos" || name == "lg2" || name == "ex2" || name == "tanh") {
        instruction_category_ = kFloatingPoint;
      }
    } else if (type[0].str().substr(1, 3) == "f64") {
      bit_width_ = 64;
      if (name == "add" || name == "sub" || name == "mul" || name == "mad" || name == "fma" || name == "div"
          || name == "rcp" || name == "sqrt" || name == "abs" || name == "neg" || name == "min" || name == "max"
          || name == "rsqrt") {
        instruction_category_ = kFloatingPoint;
      }
    }
  } else {
    // std::cout << "type not found: " << line << std::endl;
  }

  args_ = GetArgs(line);
  HandleKeyword();

  raw_line_ = std::move(line);
}

const std::string &PtxInstruction::GetRawLine() const {
  return raw_line_;
}

std::string PtxInstruction::GetDstRegister() const {
  if (args_.size() > 1) {
    return args_[1];
  } else {
    return "";
  }
}

unsigned int PtxInstruction::GetId() const {
  return id_;
}

std::vector<std::string> PtxInstruction::GetSrcRegister() const {
  std::vector<std::string> v;

  for (size_t i = 2; i < args_.size(); i++) {
    v.push_back(args_[i]);
  }

  return v;
}

std::string PtxInstruction::GetInstructionName() const {
  std::string name(args_[0]);
  size_t pos = name.find('.');
  if (pos != std::string::npos) {
    return name.substr(0, pos);
  } else {
    return name;
  }
}

bool PtxInstruction::operator<(const PtxInstruction &i) const {
  return id_ < i.GetId();
}

bool PtxInstruction::operator==(const PtxInstruction &i) const {
  return id_ == i.GetId();
}

std::ostream &operator<<(std::ostream &out, const PtxInstruction &i) {
  out << i.GetRawLine();
  if (Options::verbose) {
    out << " - ID: ";
    out << i.id_;
  }
  return out;
}

PtxInstruction::operator std::string() const {
  return GetRawLine();
}

InstructionKeyword PtxInstruction::GetInstructionKeyword() const {
  return keyword_;
}

Type PtxInstruction::GetInstructionType() const {
  return instruction_type_;
}

std::vector<std::string> PtxInstruction::GetModifier() const {
  std::string mut_line(args_[0]);
  std::vector<std::string> v;

  boost::sregex_token_iterator i(mut_line.begin(), mut_line.end(), boost::regex("[^.]+"), 0);
  boost::sregex_token_iterator end;

  // skip instruction name
  ++i;

  for (; i != end; ++i) {
    v.push_back(*i);
  }

  return v;
}

bool PtxInstruction::HasGuard() const {
  return guard_predicate_;
}

std::string PtxInstruction::GetGuardRegister() const {
  return guard_register_;
}

bool PtxInstruction::GetGuardMod() const {
  return guard_mod_;
}

void PtxInstruction::SetBlockId(unsigned int block_id) {
  block_id_ = block_id;
}

unsigned int PtxInstruction::GetBlockId() const {
  return block_id_;
}

InstructionCategory PtxInstruction::GetInstructionCategory() const {
  return instruction_category_;
}

void PtxInstruction::AddParent(const PtxInstruction &i) {
  parents_.insert(i);
}

std::set<PtxInstruction> PtxInstruction::GetParents() {
  return parents_;
}
