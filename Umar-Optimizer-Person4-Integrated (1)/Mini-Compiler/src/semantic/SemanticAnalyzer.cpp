#include "SemanticAnalyzer.h"
#include <iostream>

namespace semantic {

SemanticAnalyzer::SemanticAnalyzer() = default;

void SemanticAnalyzer::analyze(const std::vector<std::unique_ptr<ast::Statement>>& ast) {
    for (const auto& stmt : ast) {
        if (stmt) {
            stmt->accept(*this);
        }
    }
}

void SemanticAnalyzer::reportError(const std::string& message) {
    errors.push_back(message);
    std::cerr << "Semantic Error: " << message << std::endl;
}

void SemanticAnalyzer::visit(ast::BinaryExpression& node) {
    if (node.left) node.left->accept(*this);
    if (node.right) node.right->accept(*this);
}

void SemanticAnalyzer::visit(ast::LiteralExpression& node) {
    // Literals are inherently valid in this mini compiler
}

void SemanticAnalyzer::visit(ast::VariableExpression& node) {
    Symbol* sym = symbolTable.resolveVariable(node.name);
    if (!sym) {
        reportError("Use of undeclared variable: " + node.name);
    } else if (!sym->isInitialized) {
        reportError("Use of uninitialized variable: " + node.name);
    }
}

void SemanticAnalyzer::visit(ast::AssignmentStatement& node) {
    Symbol* sym = symbolTable.resolveVariable(node.variableName);
    if (!sym) {
        reportError("Assignment to undeclared variable: " + node.variableName);
    }

    if (node.value) {
        node.value->accept(*this);
        if (sym) {
            sym->isInitialized = true;
        }
    }
}

void SemanticAnalyzer::visit(ast::IfStatement& node) {
    if (node.condition) {
        node.condition->accept(*this);
    }
    if (node.thenBranch) {
        node.thenBranch->accept(*this);
    }
    if (node.elseBranch) {
        node.elseBranch->accept(*this);
    }
}

void SemanticAnalyzer::visit(ast::WhileStatement& node) {
    if (node.condition) {
        node.condition->accept(*this);
    }
    if (node.body) {
        node.body->accept(*this);
    }
}

void SemanticAnalyzer::visit(ast::VariableDeclaration& node) {
    if (!symbolTable.declareVariable(node.variableName, node.typeName)) {
        reportError("Variable redeclared in same scope: " + node.variableName);
    }

    if (node.initializer) {
        node.initializer->accept(*this);
        symbolTable.initializeVariable(node.variableName);
    }
}

void SemanticAnalyzer::visit(ast::BlockStatement& node) {
    symbolTable.enterScope();
    for (const auto& stmt : node.statements) {
        if (stmt) {
            stmt->accept(*this);
        }
    }
    symbolTable.exitScope();
}

} // namespace semantic
