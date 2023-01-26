#ifndef PTX_ANALYSER_SRC_DATA_STRUCTURES_INSTRUCTION_COUNTS_H_
#define PTX_ANALYSER_SRC_DATA_STRUCTURES_INSTRUCTION_COUNTS_H_

#include <list>

struct InstructionCounts {
  // total instrcution count
  unsigned int total_ic = 0;
  // floating point instruction count
  unsigned int fp_ic = 0;
  // executed (HyPA) instruction count
  unsigned int exec_ic = 0;
  // number of divergent branches
  unsigned int divergent_branches = 0;
  // list of the number of divergent branches in each CTA -> one entry = one CTA
  std::list<unsigned int> db_list;
};

#endif //PTX_ANALYSER_SRC_DATA_STRUCTURES_INSTRUCTION_COUNTS_H_
