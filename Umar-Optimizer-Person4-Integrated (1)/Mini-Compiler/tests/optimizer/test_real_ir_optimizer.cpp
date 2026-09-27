#include "../../src/optimizer/IROptimizer.h"
#include "../../src/optimizer/umar/RealIRBridge.h"
#include "../../src/codegen/CodeGenerator.h"
#include <cassert>
#include <climits>
#include <iostream>
static IROperand v(const char* s) { return IROperand::variable(s); }
static IROperand t(const char* s) { return IROperand::temporary(s); }
static IROperand c(const char* s) { return IROperand::constant(s); }
int main() {
    IRProgram p;
    p.emit(IRInstruction::makeMov(v("a"),c("10")));
    p.emit(IRInstruction::makeMov(v("b"),c("20")));
    p.emit(IRInstruction::makeBinary(IROpcode::MUL,t("t1"),v("b"),c("2")));
    p.emit(IRInstruction::makeBinary(IROpcode::ADD,t("t2"),v("a"),t("t1")));
    p.emit(IRInstruction::makeMov(v("x"),t("t2")));
    IROptimizer optimizer; auto out=optimizer.optimize(p);
    assert(out.toString().find("x = 50")!=std::string::npos);
    assert(out.toString().find("t1") == std::string::npos);
    assert(CodeGenerator{}.generate(out).find("MOV")!=std::string::npos);
    IRProgram branch;
    branch.emit(IRInstruction::makeMov(v("x"),c("1")));
    branch.emit(IRInstruction::makeBranch(v("condition"),"L1","L2"));
    branch.emit(IRInstruction::makeLabel("L1"));
    branch.emit(IRInstruction::makeMov(v("x"),c("2")));
    branch.emit(IRInstruction::makeJump("L3"));
    branch.emit(IRInstruction::makeLabel("L2"));
    branch.emit(IRInstruction::makeMov(v("x"),c("3")));
    branch.emit(IRInstruction::makeLabel("L3"));
    branch.emit(IRInstruction::makeBinary(IROpcode::ADD,t("t3"),v("x"),c("1")));
    branch.emit(IRInstruction::makeMov(v("y"),t("t3")));
    auto q=optimizer.optimize(branch);
    assert(q.toString().find("BRANCH condition, L1, L2")!=std::string::npos);
    assert(q.toString().find("x = 1")!=std::string::npos);
    assert(q.toString().find("t3 = x + 1")!=std::string::npos);
    assert(CodeGenerator{}.generate(q).find("BRANCH")!=std::string::npos);
    IRProgram special;
    special.emit(IRInstruction::makeMov(v("decimal"),c("3.14")));
    special.emit(IRInstruction::makeBinary(IROpcode::ADD,t("t4"),v("decimal"),c("2")));
    special.emit(IRInstruction::makeBinary(IROpcode::DIV,t("t5"),c("4"),c("0")));
    special.emit(IRInstruction::makeBinary(IROpcode::ADD,t("t6"),c("2147483647"),c("1")));
    IRInstruction neg{}; neg.opcode=IROpcode::NEG; neg.result=v("negative"); neg.operand1=v("decimal"); special.emit(neg);
    special.emit(IRInstruction::makeMov(v("result"),t("t6")));
    auto r=optimizer.optimize(special);
    assert(r.toString().find("3.14")!=std::string::npos);
    assert(r.toString().find("4 / 0")!=std::string::npos);
    assert(r.toString().find("2147483647 + 1")!=std::string::npos);
    assert(r.toString().find("negative = -decimal")!=std::string::npos);
    std::cout<<"Real IR optimizer integration tests passed\n";
}
