#ifndef PTX_ANALYSER_SRC_DATA_STRUCTURES_PTX_REGISTER_H_
#define PTX_ANALYSER_SRC_DATA_STRUCTURES_PTX_REGISTER_H_

#include <iostream>
#include <boost/any.hpp>

#include "type.h"

class PtxRegister {
  Type type_;
  boost::any value_;

  explicit PtxRegister(Type type);
 public:
  PtxRegister();
  PtxRegister(Type type, boost::any value);
  friend std::ostream &operator<<(std::ostream &out, const PtxRegister &i);

  void SetType(Type type);
  Type GetType() const;
  void SetValue(boost::any value);
  boost::any GetValue() const;
  std::string ToString() const;
};

#endif //PTX_ANALYSER_SRC_DATA_STRUCTURES_PTX_REGISTER_H_

