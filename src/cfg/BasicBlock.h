#pragma once

#include "../ir/IRInstruction.h"

#include <string>
#include <vector>

class BasicBlock {
public:
    int id;
    std::string label;

    std::vector<IRInstruction> instructions;

    std::vector<BasicBlock*> predecessors;
    std::vector<BasicBlock*> successors;

    explicit BasicBlock(int id, const std::string& label = "");

    void addInstruction(const IRInstruction& instruction);

    void addSuccessor(BasicBlock* block);
    void addPredecessor(BasicBlock* block);

    bool isEmpty() const;
    bool endsWithBranch() const;
    bool endsWithJump() const;

    std::string toString() const;
};
