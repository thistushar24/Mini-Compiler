#pragma once
#include "Statement.h"
#include "Expression.h"
#include "ASTVisitor.h"
#include <memory>

namespace ast {

class WhileStatement : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> body;

    WhileStatement(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> body)
        : condition(std::move(condition)), body(std::move(body)) {}

    void accept(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }
};

} // namespace ast
