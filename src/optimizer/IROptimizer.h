#pragma once

#include "Optimizer.h"

class IROptimizer : public Optimizer {
public:
    IRProgram optimize(const IRProgram& program) override;
};
