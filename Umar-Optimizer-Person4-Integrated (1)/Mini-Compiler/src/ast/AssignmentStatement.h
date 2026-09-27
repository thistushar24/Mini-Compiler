#pragma once
#include "Statement.h"
#include "Expression.h"
#include "ASTVisitor.h"
#include <string>
#include <memory>

namespace ast {

class AssignmentStatement : public Statement {
public:
    std::string variableName;
    std::unique_ptr<Expression> value;

    AssignmentStatement(std::string variableName, std::unique_ptr<Expression> value)
        : variableName(std::move(variableName)), value(std::move(value)) {}

    void accept(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }
};

} // namespace ast
