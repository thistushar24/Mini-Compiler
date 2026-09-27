#include "RealIRBridge.h"
#include <climits>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>
namespace umar {
namespace {
using opt::Op; using opt::Operand; using opt::Instruction;
// The temporary IR uses a reserved prefix to distinguish compiler temporaries.
std::string encode(const IROperand& x) { return x.type==IROperandType::TEMPORARY ? "$"+x.value : x.value; }
Operand input(const IROperand& x) {
    if(x.type!=IROperandType::CONSTANT) return Operand::var(encode(x));
    try {
        std::size_t n=0;
        auto v=std::stoll(x.value,&n,10);
        if(n==x.value.size() && v>=INT_MIN && v<=INT_MAX && std::to_string(v)==x.value) return Operand::constant(v);
    } catch(const std::exception&) {}
    return Operand::opaque(x.value); // decimals and unsupported literals remain unchanged
}
IROperand output(const Operand& x) {
    if(x.kind==Operand::Kind::Constant) return IROperand::constant(std::to_string(x.value));
    if(x.kind==Operand::Kind::Opaque) return IROperand::constant(x.name);
    if(x.kind==Operand::Kind::Variable) {
        if(!x.name.empty() && x.name[0]=='$') return IROperand::temporary(x.name.substr(1));
        return IROperand::variable(x.name);
    }
    return IROperand{};
}
Op convert(IROpcode o) {
    switch(o) {
    case IROpcode::MOV:return Op::Assign; case IROpcode::ADD:return Op::Add;
    case IROpcode::SUB:return Op::Sub; case IROpcode::MUL:return Op::Mul;
    case IROpcode::DIV:return Op::Div; case IROpcode::MOD:return Op::Mod;
    case IROpcode::CMP_EQ:return Op::Eq; case IROpcode::CMP_NE:return Op::Ne;
    case IROpcode::CMP_LT:return Op::Lt; case IROpcode::CMP_LE:return Op::Le;
    case IROpcode::CMP_GT:return Op::Gt; case IROpcode::CMP_GE:return Op::Ge;
    case IROpcode::LABEL:return Op::Label; case IROpcode::JUMP:return Op::Jump;
    case IROpcode::BRANCH:return Op::Branch; case IROpcode::NEG:return Op::Neg;
    }
    throw std::invalid_argument("unknown real IR opcode");
}
IROpcode restore(Op o) {
    switch(o) {
    case Op::Assign:return IROpcode::MOV; case Op::Add:return IROpcode::ADD;
    case Op::Sub:return IROpcode::SUB; case Op::Mul:return IROpcode::MUL;
    case Op::Div:return IROpcode::DIV; case Op::Mod:return IROpcode::MOD;
    case Op::Eq:return IROpcode::CMP_EQ; case Op::Ne:return IROpcode::CMP_NE;
    case Op::Lt:return IROpcode::CMP_LT; case Op::Le:return IROpcode::CMP_LE;
    case Op::Gt:return IROpcode::CMP_GT; case Op::Ge:return IROpcode::CMP_GE;
    case Op::Label:return IROpcode::LABEL; case Op::Jump:return IROpcode::JUMP;
    case Op::Branch:return IROpcode::BRANCH; case Op::Neg:return IROpcode::NEG;
    default: throw std::invalid_argument("temporary opcode cannot be restored to real IR");
    }
}
}
IRProgram RealIRBridge::optimize(const IRProgram& source,opt::OptimizationStats* stats) {
    opt::Program p;
    for(const auto& i:source.instructions) {
        Instruction x{convert(i.opcode),{}, {}, {}};
        if(i.opcode==IROpcode::LABEL || i.opcode==IROpcode::JUMP) x.dst=Operand::label(i.label);
        else if(i.opcode==IROpcode::BRANCH) {
            x.dst=Operand::label(i.trueLabel); x.a=input(i.operand1); x.b=Operand::label(i.falseLabel);
        } else {
            x.dst=Operand::var(encode(i.result)); x.a=input(i.operand1);
            if(opt::binary(x.op)) x.b=input(i.operand2);
        }
        p.instructions.push_back(std::move(x));
    }
    auto result=opt::OptimizationManager::defaults().optimize(p);
    if(stats) *stats=result;
    IRProgram out;
    for(const auto& x:p.instructions) {
        IRInstruction i{}; i.opcode=restore(x.op);
        if(x.op==Op::Label || x.op==Op::Jump) i.label=x.dst.name;
        else if(x.op==Op::Branch) { i.trueLabel=x.dst.name; i.falseLabel=x.b.name; i.operand1=output(x.a); }
        else {
            i.result=output(x.dst); i.operand1=output(x.a);
            if(opt::binary(x.op)) i.operand2=output(x.b);
        }
        out.emit(std::move(i));
    }
    return out;
}
}
