#pragma once

#include "../ir/IRProgram.h"

class Optimizer {
public:
    virtual ~Optimizer() = default;

    virtual IRProgram optimize(const IRProgram& program) = 0;
};
