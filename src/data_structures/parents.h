#ifndef PTX_ANALYSER_SRC_DATA_STRUCTURES_PARENTS_H_
#define PTX_ANALYSER_SRC_DATA_STRUCTURES_PARENTS_H_

#include <set>
#include "../ptx_structures/ptx_instruction.h"

template<typename T>
class Parents {
 public:
  virtual std::set<T> GetParents() = 0;
};

#endif //PTX_ANALYSER_SRC_DATA_STRUCTURES_PARENTS_H_
