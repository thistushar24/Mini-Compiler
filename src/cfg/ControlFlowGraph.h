#pragma once

#include "BasicBlock.h"

#include "../ir/IRProgram.h"

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

class ControlFlowGraph {
public:
    ControlFlowGraph() = default;

    void build(const IRProgram& program);

    const std::vector<std::unique_ptr<BasicBlock>>& getBlocks() const;

    BasicBlock* getEntryBlock() const;

    std::string toString() const;

private:
    std::vector<std::unique_ptr<BasicBlock>> blocks;

    BasicBlock* entryBlock = nullptr;

    std::unordered_map<std::string, BasicBlock*> labelToBlock;

    void createBlocks(const IRProgram& program);
    void connectBlocks();

    BasicBlock* findBlockByLabel(const std::string& label) const;

    bool isLeader(
        const IRInstruction& instruction,
        size_t index,
        const IRProgram& program
    ) const;
};
