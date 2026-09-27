#pragma once
#include "OptimizationPass.h"
#include <map>
#include <memory>
#include <string>
#include <vector>
namespace opt {
struct OptimizationStats { std::map<std::string,std::size_t> changes; std::size_t iterations=0; bool reachedLimit=false; };
class OptimizationManager {
    std::vector<std::unique_ptr<OptimizationPass>> passes_;
public:
    void add(std::unique_ptr<OptimizationPass> pass) { passes_.push_back(std::move(pass)); }
    static OptimizationManager defaults();
    OptimizationStats optimize(Program& program, std::size_t maxIterations=12) const;
};
std::string toString(const Program& program);
std::string report(const OptimizationStats& stats);
}
