#ifndef PTX_ANALYSER_SRC_DATA_STRUCTURES_STRING_HELPERS_H_
#define PTX_ANALYSER_SRC_DATA_STRUCTURES_STRING_HELPERS_H_

#include <string>

inline const std::string TypeToString[] =
    {"s8", "s16", "s32", "s64", "u8", "u16", "u32", "u64", "f16", "f16x2", "f32", "f64", "b8", "b16", "b32", "b64",
     "pred", "no type"};
inline const std::string StateToString[] = {"reg", "sreg", "const", "global", "local", "kernel", "shared", "tex"};

#endif //PTX_ANALYSER_SRC_DATA_STRUCTURES_STRING_HELPERS_H_

