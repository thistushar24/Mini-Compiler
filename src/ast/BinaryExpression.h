#pragma once
#include "Expression.h"
#include "ASTVisitor.h"
#include <memory>
#include "../lexer/TokenType.h"

namespace ast {

class BinaryExpression : public Expression {
public:
    std::unique_ptr<Expression> left;
    TokenType op;
    std::unique_ptr<Expression> right;

    BinaryExpression(std::unique_ptr<Expression> left, TokenType op, std::unique_ptr<Expression> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}

    void accept(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }
};

} // namespace ast
