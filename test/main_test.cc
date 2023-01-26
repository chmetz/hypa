#include "../src/parser_tools.h"
#include "../src/iss/iss.h"
#include "../src/data_structures/ptx_register.h"

#include <string>
#include <cassert>
#include <cstring>

void TestIsComment() {
  std::string s1 = "// jasdlkfnoe";
  std::string s2 = "mv dsaf,fas";
  std::string s3 = "//asfnoenfae";
  std::string s4 = "add.mul //bla,blub";

  assert(IsComment(s1));
  assert(!IsComment(s2));
  assert(IsComment(s3));
  assert(!IsComment(s4));
}

void TestTrim() {
  std::string s1 = "aneofiewaff";
  std::string s2 = " aseoafedfa";
  std::string s3 = "\taeofnwRdf";
  std::string s4 = "\t aekfjwan";
  std::string s5 = " \tewnwoenf";

  Trim(s1);
  Trim(s2);
  Trim(s3);
  Trim(s4);
  Trim(s5);

  assert(s1 == "aneofiewaff");
  assert(s2 == "aseoafedfa");
  assert(s3 == "aeofnwRdf");
  assert(s4 == "aekfjwan");
  assert(s5 == "ewnwoenf");
}

void TestIsDirective() {
  std::string s1 = ".ajwiofew";
  std::string s2 = "fjaiofjwe";
  std::string s3 = ".enfaew:fea";
  std::string s4 = "mv.asjdfe.";

  assert(IsDirective(s1));
  assert(!IsDirective(s2));
  assert(IsDirective(s3));
  assert(!IsDirective(s4));
}

void TestIsLabel() {
  std::string s1 = "jgn:";
  std::string s2 = "label";
  std::string s3 = "label: mv rd,rs1,rs2";
  std::string s4 = "mv.f32 rd,rs1,rs2";

  assert(IsLabel(s1));
  assert(!IsLabel(s2));
  assert(IsLabel(s3));
  assert(!IsLabel(s4));
}

void TestRemoveCommas() {
  std::string s1 = ",ane of iewaff";
  std::string s2 = ",as,e,oafedfa";
  std::string s3 = "aeofn, ,wRdf";
  std::string s4 = "\t aek,fjwan";
  std::string s5 = " \tewnwoenf,,";

  RemoveCommas(s1);
  RemoveCommas(s2);
  RemoveCommas(s3);
  RemoveCommas(s4);
  RemoveCommas(s5);

  assert(s1 == "ane of iewaff");
  assert(s2 == "aseoafedfa");
  assert(s3 == "aeofn wRdf");
  assert(s4 == "\t aekfjwan");
  assert(s5 == " \tewnwoenf");
}

void TestPtxInstructionGetArgs() {
  Graph<std::string> g1;
  Graph<std::string> g2;
  Graph<std::string> g3;
  Graph<std::string> g4;
  Graph<std::string> g5;

  PtxInstruction p1(0, "START: mov.b32  	r1, %tid.x;", "mov", g1);
  PtxInstruction p2(1, "START: mov.b32  	r1, %tid.x ;", "mov", g2);
  PtxInstruction p3(2, "mov r1,%tid;", "mov", g3);
  PtxInstruction p4(3, "@%p7 bra    LBB0_8;", "", g4);
  PtxInstruction p5(4, "bra.uni     LBB0_4;", "", g5);

  g1.ExportDotFile("g1.dot");
  g2.ExportDotFile("g2.dot");
  g3.ExportDotFile("g3.dot");
  g4.ExportDotFile("g4.dot");
  g5.ExportDotFile("g5.dot");

}

void TestPtxInstructionGetSrcRegister() {
  Graph<std::string> g1;
  PtxInstruction p1(0, "mov.u32 	%r6, %ctaid.x;", "mov", g1);
  PtxInstruction p2(1, "mov.u32 	%r6, %ctaid.x, %tid.x;", "mov", g1);
  PtxInstruction p3(2, "@%p1 bra 	LBB14_2;", "bra", g1);

  std::vector<std::string> p1Src = p1.GetSrcRegister();
  assert(p1Src.size() == 1);
  assert(p1Src[0] == "%ctaid.x");
  std::vector<std::string> p2Src = p2.GetSrcRegister();
  assert(p2Src.size() == 2);
  assert(p2Src[0] == "%ctaid.x");
  assert(p2Src[1] == "%tid.x");
  std::vector<std::string> p3Src = p3.GetSrcRegister();
  assert(p3Src.size() == 1);
  assert(p3Src[0] == "%p1");
}

void TestIssAddInstruction() {
  Graph<std::string> g1;
  PtxInstruction p1(0, "add.s64 %rd1, %rd2, %rd3;", "add", g1);

  std::set<PtxInstruction> executionList;
  executionList.insert(p1);

  Iss i;

  std::map<std::string, PtxRegister> regs;
  regs.insert(std::pair<std::string, PtxRegister>("%rd1", PtxRegister(kS64, 0)));
  regs.insert(std::pair<std::string, PtxRegister>("%rd2", PtxRegister(kS64, 2)));
  regs.insert(std::pair<std::string, PtxRegister>("%rd3", PtxRegister(kS64, 200)));

  i.Execute(executionList, regs);

  /*for (const auto &[name, reg] : regs) {
    std::cout << "  " << name << " " << reg << "\n";
  }*/

}

void TestIssSmallPtx() {
  Graph<std::string> g1;

  PtxInstruction p0(0, "mov.u32 	%r6, %ctaid.x;", "mov", g1);
  PtxInstruction p1(1, "mov.u32 	%r7, %tid.x;", "mov", g1);
  PtxInstruction p2(2, "shl.b32 	%r8, %r6, 9;", "shl", g1);
  PtxInstruction p3(3, "shl.b32 	%r1, %r7, 2;", "shl", g1);
  PtxInstruction p4(4, "or.b32  	%r2, %r1, %r8;", "or", g1);
  PtxInstruction p5(5, "setp.lt.u32 	%p1, %r2, 16384;", "setp", g1);

  std::set<PtxInstruction> executionList;
  executionList.insert(p0);
  executionList.insert(p1);
  executionList.insert(p2);
  executionList.insert(p3);
  executionList.insert(p4);
  executionList.insert(p5);

  std::map<std::string, PtxRegister> regs;
  regs.insert(std::pair<std::string, PtxRegister>("%ctaid.x", PtxRegister(kU32, static_cast<uint32_t>(2))));
  regs.insert(std::pair<std::string, PtxRegister>("%tid.x", PtxRegister(kU32, static_cast<uint32_t>(1))));

  regs.insert(std::pair<std::string, PtxRegister>("%r1", PtxRegister(kB32, static_cast<uint32_t>(0))));
  regs.insert(std::pair<std::string, PtxRegister>("%r2", PtxRegister(kB32, static_cast<uint32_t>(0))));
  regs.insert(std::pair<std::string, PtxRegister>("%r6", PtxRegister(kU32, static_cast<uint32_t>(0))));
  regs.insert(std::pair<std::string, PtxRegister>("%r7", PtxRegister(kU32, static_cast<uint32_t>(0))));
  regs.insert(std::pair<std::string, PtxRegister>("%r8", PtxRegister(kB32, static_cast<uint32_t>(0))));
  regs.insert(std::pair<std::string, PtxRegister>("%p1", PtxRegister(kPred, false)));

  regs.insert(std::pair<std::string, PtxRegister>("9", PtxRegister(kU32, static_cast<uint32_t>(9))));
  regs.insert(std::pair<std::string, PtxRegister>("2", PtxRegister(kU32, static_cast<uint32_t>(2))));
  regs.insert(std::pair<std::string, PtxRegister>("16384", PtxRegister(kU32, static_cast<uint32_t>(16384))));

  Iss::Execute(executionList, regs);

  for (const auto &[name, reg] : regs) {
    std::cout << "  " << name << " " << reg << "\n";
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    TestIsComment();
    TestTrim();
    TestIsDirective();
    TestIsLabel();
    TestRemoveCommas();
    TestPtxInstructionGetArgs();
    TestPtxInstructionGetSrcRegister();
    //TestIssAddInstruction();
    TestIssSmallPtx();

    return 0;
  }

  if (strcmp(argv[1], "0") == 0) {
    TestIsComment();
  } else if (strcmp(argv[1], "1") == 0) {
    TestTrim();
  } else if (strcmp(argv[1], "2") == 0) {
    TestIsDirective();
  } else if (strcmp(argv[1], "3") == 0) {
    TestIsLabel();
  } else if (strcmp(argv[1], "4") == 0) {
    TestRemoveCommas();
  } else if (strcmp(argv[1], "5") == 0) {
    TestPtxInstructionGetArgs();
  } else if (strcmp(argv[1], "6") == 0) {
    TestPtxInstructionGetSrcRegister();
  } else if (strcmp(argv[1], "7") == 0) {
    TestIssAddInstruction();
  } else if (strcmp(argv[1], "8") == 0) {
    TestIssSmallPtx();
  }

  return 0;
}

