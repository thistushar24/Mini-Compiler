#include "OptimizationManager.h"
#include "Passes.h"
#include <sstream>
#include <stdexcept>
namespace opt {
OptimizationManager OptimizationManager::defaults() {
    OptimizationManager m;
    m.add(std::make_unique<ConstantPropagation>());
    m.add(std::make_unique<ConstantFolding>());
    m.add(std::make_unique<AlgebraicSimplification>());
    m.add(std::make_unique<CommonSubexpression>());
    m.add(std::make_unique<DeadCodeElimination>());
    return m;
}
OptimizationStats OptimizationManager::optimize(Program& p,std::size_t limit) const {
    OptimizationStats s;
    if(limit==0) throw std::invalid_argument("maxIterations must be positive");
    for(std::size_t n=0;n<limit;++n) {
        bool changed=false;
        for(const auto& pass:passes_) { auto r=pass->run(p); s.changes[pass->name()]+=r.count; changed|=r.changed; }
        ++s.iterations;
        if(!changed) return s;
    }
    s.reachedLimit=true;
    return s;
}
namespace {
std::string operand(const Operand& a) {
    switch(a.kind) { case Operand::Kind::Constant: return std::to_string(a.value);
    case Operand::Kind::Variable: case Operand::Kind::Label: case Operand::Kind::Opaque: return a.name;
    default: return "?"; }
}
const char* symbol(Op o) {
    switch(o) { case Op::Add:return "+"; case Op::Sub:return "-"; case Op::Mul:return "*";
    case Op::Div:return "/"; case Op::Mod:return "%"; case Op::Eq:return "==";
    case Op::Ne:return "!="; case Op::Lt:return "<"; case Op::Le:return "<=";
    case Op::Gt:return ">"; case Op::Ge:return ">="; default:return "?"; }
}
}
std::string toString(const Program& p) {
    std::ostringstream out;
    for(const auto& i:p.instructions) {
        if(i.op==Op::Neg) out<<operand(i.dst)<<" = -"<<operand(i.a)<<'\n';
        else if(i.op==Op::Branch) out<<"branch "<<operand(i.a)<<", "<<operand(i.dst)<<", "<<operand(i.b)<<'\n';
        else if(i.op==Op::Label) out<<operand(i.dst)<<":\n";
        else if(i.op==Op::Jump) out<<"goto "<<operand(i.dst)<<'\n';
        else if(i.op==Op::JumpIfFalse) out<<"ifFalse "<<operand(i.a)<<" goto "<<operand(i.dst)<<'\n';
        else if(i.op==Op::Print) out<<"print "<<operand(i.a)<<'\n';
        else if(i.op==Op::Assign) out<<operand(i.dst)<<" = "<<operand(i.a)<<'\n';
        else out<<operand(i.dst)<<" = "<<operand(i.a)<<' '<<symbol(i.op)<<' '<<operand(i.b)<<'\n';
    }
    return out.str();
}
std::string report(const OptimizationStats& s) {
    std::ostringstream out;
    out<<"Iterations: "<<s.iterations<<"\n";
    for(auto& [name,count]:s.changes) out<<name<<": "<<count<<"\n";
    if(s.reachedLimit) out<<"Iteration limit reached\n";
    return out.str();
}
}
