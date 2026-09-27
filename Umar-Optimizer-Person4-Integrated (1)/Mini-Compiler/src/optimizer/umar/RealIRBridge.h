#pragma once
#include "../../ir/IRProgram.h"
#include "OptimizationManager.h"
namespace umar {
class RealIRBridge {
public:
    static IRProgram optimize(const IRProgram& source, opt::OptimizationStats* stats=nullptr);
};
}
