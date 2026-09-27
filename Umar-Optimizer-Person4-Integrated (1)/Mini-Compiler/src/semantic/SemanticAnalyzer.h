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
#include "SymbolTable.h"
#include <vector>
#include <memory>
#include <string>

namespace semantic {

class SemanticAnalyzer : public ast::ASTVisitor {
public:
    SemanticAnalyzer();

    void analyze(const std::vector<std::unique_ptr<ast::Statement>>& ast);

    // Visitor methods
    void visit(ast::BinaryExpression& node) override;
    void visit(ast::LiteralExpression& node) override;
    void visit(ast::VariableExpression& node) override;
    void visit(ast::AssignmentStatement& node) override;
    void visit(ast::IfStatement& node) override;
    void visit(ast::WhileStatement& node) override;
    void visit(ast::VariableDeclaration& node) override;
    void visit(ast::BlockStatement& node) override;

    bool hasErrors() const { return !errors.empty(); }
    const std::vector<std::string>& getErrors() const { return errors; }

private:
    SymbolTable symbolTable;
    std::vector<std::string> errors;

    void reportError(const std::string& message);
};

} // namespace semantic
