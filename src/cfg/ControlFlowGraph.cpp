#include "ControlFlowGraph.h"

#include <sstream>
#include <stdexcept>

void ControlFlowGraph::build(const IRProgram& program) {
    blocks.clear();
    labelToBlock.clear();
    entryBlock = nullptr;

    createBlocks(program);
    connectBlocks();
}

const std::vector<std::unique_ptr<BasicBlock>>&
ControlFlowGraph::getBlocks() const {
    return blocks;
}

BasicBlock* ControlFlowGraph::getEntryBlock() const {
    return entryBlock;
}

bool ControlFlowGraph::isLeader(
    const IRInstruction& instruction,
    size_t index,
    const IRProgram& program
) const {
    if (index == 0) {
        return true;
    }

    if (instruction.opcode == IROpcode::LABEL) {
        return true;
    }

    const IRInstruction& previous = program.instructions[index - 1];

    if (previous.opcode == IROpcode::JUMP ||
        previous.opcode == IROpcode::BRANCH) {
        return true;
    }

    return false;
}

void ControlFlowGraph::createBlocks(const IRProgram& program) {
    if (program.instructions.empty()) {
        return;
    }

    BasicBlock* currentBlock = nullptr;
    int blockId = 0;

    for (size_t i = 0; i < program.instructions.size(); ++i) {
        const IRInstruction& instruction = program.instructions[i];

        if (isLeader(instruction, i, program)) {
            std::string label;

            if (instruction.opcode == IROpcode::LABEL) {
                label = instruction.label;
            }

            blocks.push_back(
                std::make_unique<BasicBlock>(blockId++, label)
            );

            currentBlock = blocks.back().get();

            if (entryBlock == nullptr) {
                entryBlock = currentBlock;
            }

            if (!label.empty()) {
                labelToBlock[label] = currentBlock;
            }
        }

        currentBlock->addInstruction(instruction);
    }
}

BasicBlock* ControlFlowGraph::findBlockByLabel(
    const std::string& label
) const {
    auto it = labelToBlock.find(label);

    if (it == labelToBlock.end()) {
        return nullptr;
    }

    return it->second;
}

void ControlFlowGraph::connectBlocks() {
    for (size_t i = 0; i < blocks.size(); ++i) {
        BasicBlock* block = blocks[i].get();

        if (block->instructions.empty()) {
            continue;
        }

        const IRInstruction& last =
            block->instructions.back();

        if (last.opcode == IROpcode::BRANCH) {
            BasicBlock* trueBlock =
                findBlockByLabel(last.trueLabel);

            BasicBlock* falseBlock =
                findBlockByLabel(last.falseLabel);

            if (trueBlock != nullptr) {
                block->addSuccessor(trueBlock);
                trueBlock->addPredecessor(block);
            }

            if (falseBlock != nullptr) {
                block->addSuccessor(falseBlock);
                falseBlock->addPredecessor(block);
            }

            continue;
        }

        if (last.opcode == IROpcode::JUMP) {
            BasicBlock* target =
                findBlockByLabel(last.label);

            if (target != nullptr) {
                block->addSuccessor(target);
                target->addPredecessor(block);
            }

            continue;
        }

        if (i + 1 < blocks.size()) {
            BasicBlock* nextBlock = blocks[i + 1].get();

            block->addSuccessor(nextBlock);
            nextBlock->addPredecessor(block);
        }
    }
}

std::string ControlFlowGraph::toString() const {
    std::ostringstream output;

    output << "===== CONTROL FLOW GRAPH =====\n";

    for (const auto& block : blocks) {
        output << block->toString();
        output << "\n";
    }

    output << "==============================\n";

    return output.str();
}
