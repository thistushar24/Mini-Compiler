#pragma once
// Temporary optimizer-facing IR contract. Map Person 3's IR here at integration time.
#include <cstdint>
#include <string>
#include <vector>
namespace opt {
enum class Op { Assign, Add, Sub, Mul, Div, Mod, Eq, Ne, Lt, Le, Gt, Ge, Label, Jump, JumpIfFalse, Print, Neg, Branch };
struct Operand {
    enum class Kind { None, Variable, Constant, Label, Opaque } kind = Kind::None;
    std::string name;
    std::int64_t value = 0;
    static Operand var(std::string n) { return {Kind::Variable, std::move(n), 0}; }
    static Operand constant(std::int64_t v) { return {Kind::Constant, "", v}; }
    static Operand opaque(std::string n) { return {Kind::Opaque, std::move(n), 0}; }
    static Operand label(std::string n) { return {Kind::Label, std::move(n), 0}; }
    bool operator==(const Operand& b) const { return kind==b.kind && name==b.name && value==b.value; }
};
struct Instruction {
    Op op; Operand dst, a, b;
    bool operator==(const Instruction& x) const { return op==x.op && dst==x.dst && a==x.a && b==x.b; }
};
struct Program { std::vector<Instruction> instructions; };
inline bool writes(Op o) { return o <= Op::Ge || o==Op::Neg; }
inline bool binary(Op o) { return o >= Op::Add && o <= Op::Ge; }
inline bool barrier(Op o) { return o==Op::Label || o==Op::Jump || o==Op::JumpIfFalse || o==Op::Branch || o==Op::Neg; }
inline bool sideEffect(Op o) { return o==Op::Print || o==Op::Jump || o==Op::JumpIfFalse || o==Op::Label || o==Op::Branch; }
}
