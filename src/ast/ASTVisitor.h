#pragma once

namespace ast {

// Forward declarations of all concrete AST nodes
class BinaryExpression;
class LiteralExpression;
class VariableExpression;
class AssignmentStatement;
class IfStatement;
class WhileStatement;
class VariableDeclaration;
class BlockStatement;

class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;

    virtual void visit(BinaryExpression& node) = 0;
    virtual void visit(LiteralExpression& node) = 0;
    virtual void visit(VariableExpression& node) = 0;
    virtual void visit(AssignmentStatement& node) = 0;
    virtual void visit(IfStatement& node) = 0;
    virtual void visit(WhileStatement& node) = 0;
    virtual void visit(VariableDeclaration& node) = 0;
    virtual void visit(BlockStatement& node) = 0;
};

} // namespace ast
