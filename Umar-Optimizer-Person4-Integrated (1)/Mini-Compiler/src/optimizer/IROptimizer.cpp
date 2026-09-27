#include "IROptimizer.h"
#include "umar/RealIRBridge.h"
IRProgram IROptimizer::optimize(const IRProgram& program) {
    return umar::RealIRBridge::optimize(program);
}
