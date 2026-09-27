#pragma once
#include "ASTNode.h"

namespace ast {

class Expression : public ASTNode {
public:
    virtual ~Expression() = default;
    // Expressions will have a type (e.g., INT) after semantic analysis,
    // but for now, they just represent an evaluatable node.
};

} // namespace ast
