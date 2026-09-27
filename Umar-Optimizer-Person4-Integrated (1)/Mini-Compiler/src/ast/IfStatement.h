#pragma once
#include "Statement.h"
#include "Expression.h"
#include "ASTVisitor.h"
#include <memory>

namespace ast {

class IfStatement : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> thenBranch;
    std::unique_ptr<Statement> elseBranch;

    IfStatement(std::unique_ptr<Expression> condition, 
                std::unique_ptr<Statement> thenBranch, 
                std::unique_ptr<Statement> elseBranch = nullptr)
        : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}

    void accept(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }
};

} // namespace ast
