#pragma once
#include "ASTNode.h"

namespace ast {

class Statement : public ASTNode {
public:
    virtual ~Statement() = default;
};

} // namespace ast
