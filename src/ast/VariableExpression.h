#pragma once
#include "Expression.h"
#include "ASTVisitor.h"
#include <string>

namespace ast {

class VariableExpression : public Expression {
public:
    std::string name;

    explicit VariableExpression(std::string name) : name(std::move(name)) {}

    void accept(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }
};

} // namespace ast
