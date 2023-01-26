cmake_minimum_required(VERSION 3.21)
project(ptx_analyser)

set(CMAKE_CXX_STANDARD 20)

if(DEBUG)
  add_compile_options(-g -Wall -Wextra)
endif()

set(SOURCES
    src/ptx_structures/ptx_instruction.cc
    src/ptx_structures/ptx_instruction.h
    src/ptx_structures/ptx_program.cc
    src/ptx_structures/ptx_program.h
    src/parser_tools.cc
    src/parser_tools.h
    src/ptx_structures/ptx_part.cc
    src/ptx_structures/ptx_part.h
    src/data_structures/instruction_category.h
    src/data_structures/graph.h
    src/data_structures/instruction_keyword.h
    src/iss/iss.cc
    src/iss/iss.h
    src/iss/iss_pred.cc
    src/iss/iss_u8.cc
    src/iss/iss_ub16.cc
    src/iss/iss_ub32.cc
    src/iss/iss_ub64.cc
    src/iss/iss_s8.cc
    src/iss/iss_s16.cc
    src/iss/iss_s32.cc
    src/iss/iss_s64.cc
    src/data_structures/ptx_register.h
    src/data_structures/ptx_register.cc
    src/data_structures/options.h
    src/logger.cc
    src/logger.h
    src/data_structures/string_helpers.h
    src/data_structures/instruction_counts.h
    src/data_structures/parents.h)

add_executable(hypa ${SOURCES} src/main.cc)

if(CMAKE_PROJECT_NAME STREQUAL ptx_analyser AND BUILD_TESTS)
  enable_testing()
  add_executable(test_ptx_analyser ${SOURCES} test/main_test.cc)

  add_test(
    NAME IsComment
    COMMAND test_ptx_analyser 0
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(
    NAME Trim
    COMMAND test_ptx_analyser 1
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(
    NAME IsDirective
    COMMAND test_ptx_analyser 2
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(
    NAME IsLabel
    COMMAND test_ptx_analyser 3
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(
    NAME RemoveCommas
    COMMAND test_ptx_analyser 4
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(
    NAME PtxInstructionGetArgs
    COMMAND test_ptx_analyser 5
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(
    NAME PtxInstructionGetSrcRegister
    COMMAND test_ptx_analyser 6
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  add_test(
    NAME IssAddInstruction
    COMMAND test_ptx_analyser 7
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endif()
