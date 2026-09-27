#pragma once
#include "OptimizationPass.h"
namespace opt {
class ConstantFolding : public OptimizationPass { public: const char* name() const override { return "Constant folding"; } PassResult run(Program&) const override; };
class ConstantPropagation : public OptimizationPass { public: const char* name() const override { return "Constant propagation"; } PassResult run(Program&) const override; };
class AlgebraicSimplification : public OptimizationPass { public: const char* name() const override { return "Algebraic simplification"; } PassResult run(Program&) const override; };
class DeadCodeElimination : public OptimizationPass { public: const char* name() const override { return "Dead code elimination"; } PassResult run(Program&) const override; };
class CommonSubexpression : public OptimizationPass { public: const char* name() const override { return "Common subexpression elimination"; } PassResult run(Program&) const override; };
}
