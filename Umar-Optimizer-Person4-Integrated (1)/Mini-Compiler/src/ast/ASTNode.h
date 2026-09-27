#pragma once

namespace ast {

class ASTVisitor; // Forward declaration

class ASTNode {
public:
    virtual ~ASTNode() = default;
    
    // We'll use the Visitor pattern to allow SemanticAnalyzer (Person 2) 
    // and IRGenerator (Person 3) to traverse the tree cleanly.
    virtual void accept(ASTVisitor& visitor) = 0;
};

} // namespace ast
