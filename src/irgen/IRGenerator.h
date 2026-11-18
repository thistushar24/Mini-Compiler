#pragma once

#include "../ast/ASTVisitor.h"
#include "../ast/BinaryExpression.h"
#include "../ast/LiteralExpression.h"
#include "../ast/VariableExpression.h"
#include "../ast/AssignmentStatement.h"
#include "../ast/IfStatement.h"
#include "../ast/WhileStatement.h"
#include "../ast/VariableDeclaration.h"
#include "../ast/BlockStatement.h"

#include "../ir/IRProgram.h"

#include <memory>
#include <string>
#include <vector>

class IRGenerator : public ast::ASTVisitor {
public:
    IRProgram generate(
        const std::vector<std::unique_ptr<ast::Statement>>& statements
    );

    void visit(ast::BinaryExpression& node) override;
    void visit(ast::LiteralExpression& node) override;
    void visit(ast::VariableExpression& node) override;
    void visit(ast::AssignmentStatement& node) override;
    void visit(ast::IfStatement& node) override;
    void visit(ast::WhileStatement& node) override;
    void visit(ast::VariableDeclaration& node) override;
    void visit(ast::BlockStatement& node) override;

private:
    IRProgram program;

    int temporaryCounter = 0;
    int labelCounter = 0;

    std::string lastValue;

    std::string newTemporary();
    std::string newLabel();

    std::string generateExpression(ast::Expression& expression);

    IROperand makeOperand(const std::string& value) const;
};
