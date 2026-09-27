#pragma once
#include "Expression.h"
#include "ASTVisitor.h"
#include <string>

namespace ast {

class LiteralExpression : public Expression {
public:
    std::string value;
    
    explicit LiteralExpression(std::string value) : value(std::move(value)) {}

    void accept(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }
};

} // namespace ast
