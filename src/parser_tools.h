#ifndef PTX_ANALYSER_SRC_PARSER_TOOLS_H_
#define PTX_ANALYSER_SRC_PARSER_TOOLS_H_

#include <string>
#include "ptx_structures/ptx_program.h"

std::string &Trim(std::string &s);
std::string &RemoveCommas(std::string &s);
bool IsComment(const std::string &line);
bool IsDirective(const std::string &line);
bool IsLabel(const std::string &line);
void handle_line(const std::string &line, PtxProgram &ptx);
size_t NextArgWhitespaceSeperated(const std::string &s);
Type GetType(const std::string &s);

#endif //PTX_ANALYSER_SRC_PARSER_TOOLS_H_

