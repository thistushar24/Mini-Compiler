#pragma once
#include "Statement.h"
#include "ASTVisitor.h"
#include <memory>
#include <vector>

namespace ast {

class BlockStatement : public Statement {
public:
    std::vector<std::unique_ptr<Statement>> statements;

    explicit BlockStatement(std::vector<std::unique_ptr<Statement>> statements)
        : statements(std::move(statements)) {}

    void accept(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }
};

} // namespace ast
