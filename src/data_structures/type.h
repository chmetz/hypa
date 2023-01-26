#ifndef PTX_ANALYSER_SRC_DATA_STRUCTURES_TYPE_H_
#define PTX_ANALYSER_SRC_DATA_STRUCTURES_TYPE_H_

enum Type {
  // Signed Integer
  kS8,     // 0
  kS16,    // 1
  kS32,    // 2
  kS64,    // 3

  // Unsigned Integer
  kU8,     // 4
  kU16,    // 5
  kU32,    // 6
  kU64,    // 7

  // Floating Point
  kF16,    // 8
  kF16X2,  // 9
  kF32,    // 10
  kF64,    // 11

  // Bits (untyped)
  kB8,     // 12
  kB16,    // 13
  kB32,    // 14
  kB64,    // 15

  // Predicate
  kPred,   // 16

  // NoType
  kNoType, // 17
};

#endif //PTX_ANALYSER_SRC_DATA_STRUCTURES_TYPE_H_

