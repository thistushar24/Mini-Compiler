#pragma once
#include "Statement.h"
#include "Expression.h"
#include "ASTVisitor.h"
#include <string>
#include <memory>

namespace ast {

class VariableDeclaration : public Statement {
public:
    std::string typeName;
    std::string variableName;
    std::unique_ptr<Expression> initializer;

    VariableDeclaration(std::string typeName, std::string variableName, std::unique_ptr<Expression> initializer = nullptr)
        : typeName(std::move(typeName)), variableName(std::move(variableName)), initializer(std::move(initializer)) {}

    void accept(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }
};

} // namespace ast
