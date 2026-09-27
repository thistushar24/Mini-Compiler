#include "Passes.h"
#include <algorithm>
#include <limits>
#include <climits>
#include <map>
#include <set>
#include <tuple>
#include <unordered_map>
#include <utility>
namespace opt {
namespace {
using K=Operand::Kind;
bool c(const Operand& a, std::int64_t v) { return a.kind==K::Constant && a.value==v; }
bool var(const Operand& a) { return a.kind==K::Variable; }
void assign(Instruction& i, Operand a) { i.op=Op::Assign; i.a=std::move(a); i.b={}; }
bool checked(std::int64_t v, std::int64_t& out) { if(v<INT_MIN || v>INT_MAX) return false; out=static_cast<std::int64_t>(v); return true; }
bool evaluate(Op op, std::int64_t a, std::int64_t b, std::int64_t& out) {
    switch(op) {
    case Op::Add: return checked(a+b,out);
    case Op::Sub: return checked(a-b,out);
    case Op::Mul: return checked(a*b,out);
    case Op::Div: if(!b || (a==INT64_MIN && b==-1)) return false; out=a/b; return true;
    case Op::Mod: if(!b || (a==INT64_MIN && b==-1)) return false; out=a%b; return true;
    case Op::Eq: out=a==b; return true; case Op::Ne: out=a!=b; return true;
    case Op::Lt: out=a<b; return true; case Op::Le: out=a<=b; return true;
    case Op::Gt: out=a>b; return true; case Op::Ge: out=a>=b; return true;
    default: return false;
    }
}
bool readsB(Op op) { return binary(op); }
std::vector<std::pair<std::size_t,std::size_t>> blocks(const Program& p) {
    std::vector<std::pair<std::size_t,std::size_t>> result;
    std::size_t start=0;
    for(std::size_t j=0;j<p.instructions.size();++j) {
        if(p.instructions[j].op==Op::Label && j>start) { result.emplace_back(start,j); start=j; }
        if((p.instructions[j].op==Op::Jump || p.instructions[j].op==Op::JumpIfFalse || p.instructions[j].op==Op::Branch) && j+1>start) { result.emplace_back(start,j+1); start=j+1; }
    }
    if(start<p.instructions.size()) result.emplace_back(start,p.instructions.size());
    return result;
}
}
PassResult ConstantFolding::run(Program& p) const {
    PassResult r;
    for(auto& i:p.instructions) if(binary(i.op) && i.a.kind==K::Constant && i.b.kind==K::Constant) {
        std::int64_t value;
        if(evaluate(i.op,i.a.value,i.b.value,value)) { assign(i,Operand::constant(value)); r.changed=true; ++r.count; }
    }
    return r;
}
PassResult AlgebraicSimplification::run(Program& p) const {
    PassResult r;
    for(auto& i:p.instructions) {
        Operand replacement; bool ok=true;
        switch(i.op) {
        case Op::Add: if(c(i.a,0)) replacement=i.b; else if(c(i.b,0)) replacement=i.a; else ok=false; break;
        case Op::Sub: if(c(i.b,0)) replacement=i.a; else ok=false; break;
        case Op::Mul: if(c(i.a,0)||c(i.b,0)) replacement=Operand::constant(0);
            else if(c(i.a,1)) replacement=i.b;
            else if(c(i.b,1)) replacement=i.a;
            else ok=false;
            break;
        case Op::Div: if(c(i.b,1)) replacement=i.a; else ok=false; break;
        default: ok=false;
        }
        // Arithmetic is pure in the temporary IR, but division by zero and overflow remain observable.
        if(ok && !(i.op==Op::Div && c(i.a,INT64_MIN))) { assign(i,replacement); r.changed=true; ++r.count; }
    }
    return r;
}
PassResult ConstantPropagation::run(Program& p) const {
    PassResult r;
    for(auto [start,end]:blocks(p)) {
        std::unordered_map<std::string,std::int64_t> known;
        for(std::size_t j=start;j<end;++j) {
            auto& i=p.instructions[j];
            auto substitute=[&](Operand& x) { auto it=known.find(x.name); if(var(x) && it!=known.end()) { x=Operand::constant(it->second); ++r.count; r.changed=true; } };
            if(writes(i.op)||i.op==Op::Print||i.op==Op::JumpIfFalse || i.op==Op::Branch) substitute(i.a);
            if(readsB(i.op)) substitute(i.b);
            if(writes(i.op) && var(i.dst)) {
                if(i.op==Op::Assign && i.a.kind==K::Constant) known[i.dst.name]=i.a.value;
                else known.erase(i.dst.name);
            }
        }
    }
    return r;
}
PassResult CommonSubexpression::run(Program& p) const {
    PassResult r;
    for(auto [start,end]:blocks(p)) {
        using Key=std::tuple<Op,K,std::string,std::int64_t,K,std::string,std::int64_t>;
        std::map<Key,std::string> available;
        for(std::size_t j=start;j<end;++j) {
            auto& i=p.instructions[j];
            if(writes(i.op) && var(i.dst)) {
                const auto& dst=i.dst.name;
                for(auto it=available.begin();it!=available.end();) {
                    const auto& k=it->first;
                    if(it->second==dst || (std::get<1>(k)==K::Variable && std::get<2>(k)==dst) || (std::get<4>(k)==K::Variable && std::get<5>(k)==dst)) it=available.erase(it);
                    else ++it;
                }
            }
            // Div/Mod can fail. Only eliminate operations that are total for all operands.
            if(!binary(i.op) || i.op==Op::Div || i.op==Op::Mod || !var(i.dst)) continue;
            Key k{i.op,i.a.kind,i.a.name,i.a.value,i.b.kind,i.b.name,i.b.value};
            auto it=available.find(k);
            if(it!=available.end()) { assign(i,Operand::var(it->second)); ++r.count; r.changed=true; }
            else available.emplace(std::move(k),i.dst.name);
        }
    }
    return r;
}
PassResult DeadCodeElimination::run(Program& p) const {
    PassResult result;
    // Keep all user-visible variable values live at block exit. Only temporary names beginning
    // with '$' are compiler-owned; their unused assignments can be removed within straight-line IR.
    if(std::any_of(p.instructions.begin(),p.instructions.end(),[](const Instruction& i){ return barrier(i.op); })) return result;
    std::vector<bool> remove(p.instructions.size(),false);
    for(auto [start,end]:blocks(p)) {
        std::set<std::string> live;
        for(std::size_t j=end;j-->start;) {
            const auto& i=p.instructions[j];
            bool temp=var(i.dst) && !i.dst.name.empty() && i.dst.name[0]=='$';
            bool discard=writes(i.op) && temp && !live.count(i.dst.name) && i.op!=Op::Div && i.op!=Op::Mod;
            if(discard) { remove[j]=true; ++result.count; result.changed=true; continue; }
            if(writes(i.op) && var(i.dst)) live.erase(i.dst.name);
            if((writes(i.op)||i.op==Op::Print||i.op==Op::JumpIfFalse) && var(i.a)) live.insert(i.a.name);
            if(readsB(i.op) && var(i.b)) live.insert(i.b.name);
        }
    }
    if(result.changed) {
        std::vector<Instruction> kept;
        for(std::size_t j=0;j<p.instructions.size();++j) if(!remove[j]) kept.push_back(p.instructions[j]);
        p.instructions=std::move(kept);
    }
    return result;
}
}
