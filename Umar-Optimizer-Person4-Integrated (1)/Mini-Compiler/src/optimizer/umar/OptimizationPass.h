#pragma once
#include "IRAdapter.h"
#include <cstddef>
namespace opt {
struct PassResult { bool changed=false; std::size_t count=0; };
class OptimizationPass {
public:
    virtual ~OptimizationPass() = default;
    virtual const char* name() const = 0;
    virtual PassResult run(Program& program) const = 0;
};
}
