#include "BasicBlock.h"

#include <sstream>

BasicBlock::BasicBlock(int id, const std::string& label)
    : id(id), label(label) {
}

void BasicBlock::addInstruction(const IRInstruction& instruction) {
    instructions.push_back(instruction);
}

void BasicBlock::addSuccessor(BasicBlock* block) {
    if (block == nullptr) {
        return;
    }

    for (BasicBlock* existing : successors) {
        if (existing == block) {
            return;
        }
    }

    successors.push_back(block);
}

void BasicBlock::addPredecessor(BasicBlock* block) {
    if (block == nullptr) {
        return;
    }

    for (BasicBlock* existing : predecessors) {
        if (existing == block) {
            return;
        }
    }

    predecessors.push_back(block);
}

bool BasicBlock::isEmpty() const {
    return instructions.empty();
}

bool BasicBlock::endsWithBranch() const {
    if (instructions.empty()) {
        return false;
    }

    return instructions.back().opcode == IROpcode::BRANCH;
}

bool BasicBlock::endsWithJump() const {
    if (instructions.empty()) {
        return false;
    }

    return instructions.back().opcode == IROpcode::JUMP;
}

std::string BasicBlock::toString() const {
    std::ostringstream output;

    output << "Block " << id;

    if (!label.empty()) {
        output << " [" << label << "]";
    }

    output << ":\n";

    for (const auto& instruction : instructions) {
        output << "    ";

        switch (instruction.opcode) {
            case IROpcode::MOV:
                output << instruction.result.toString()
                       << " = "
                       << instruction.operand1.toString();
                break;

            case IROpcode::ADD:
                output << instruction.result.toString()
                       << " = "
                       << instruction.operand1.toString()
                       << " + "
                       << instruction.operand2.toString();
                break;

            case IROpcode::SUB:
                output << instruction.result.toString()
                       << " = "
                       << instruction.operand1.toString()
                       << " - "
                       << instruction.operand2.toString();
                break;

            case IROpcode::MUL:
                output << instruction.result.toString()
                       << " = "
                       << instruction.operand1.toString()
                       << " * "
                       << instruction.operand2.toString();
                break;

            case IROpcode::DIV:
                output << instruction.result.toString()
                       << " = "
                       << instruction.operand1.toString()
                       << " / "
                       << instruction.operand2.toString();
                break;

            case IROpcode::MOD:
                output << instruction.result.toString()
                       << " = "
                       << instruction.operand1.toString()
                       << " % "
                       << instruction.operand2.toString();
                break;

            case IROpcode::CMP_EQ:
                output << instruction.result.toString()
                       << " = "
                       << instruction.operand1.toString()
                       << " == "
                       << instruction.operand2.toString();
                break;

            case IROpcode::CMP_NE:
                output << instruction.result.toString()
                       << " = "
                       << instruction.operand1.toString()
                       << " != "
                       << instruction.operand2.toString();
                break;

            case IROpcode::CMP_LT:
                output << instruction.result.toString()
                       << " = "
                       << instruction.operand1.toString()
                       << " < "
                       << instruction.operand2.toString();
                break;

            case IROpcode::CMP_GT:
                output << instruction.result.toString()
                       << " = "
                       << instruction.operand1.toString()
                       << " > "
                       << instruction.operand2.toString();
                break;

            case IROpcode::CMP_LE:
                output << instruction.result.toString()
                       << " = "
                       << instruction.operand1.toString()
                       << " <= "
                       << instruction.operand2.toString();
                break;

            case IROpcode::CMP_GE:
                output << instruction.result.toString()
                       << " = "
                       << instruction.operand1.toString()
                       << " >= "
                       << instruction.operand2.toString();
                break;

            case IROpcode::NEG:
                output << instruction.result.toString()
                       << " = -"
                       << instruction.operand1.toString();
                break;

            case IROpcode::LABEL:
                output << instruction.label << ":";
                break;

            case IROpcode::JUMP:
                output << "JUMP " << instruction.label;
                break;

            case IROpcode::BRANCH:
                output << "BRANCH "
                       << instruction.operand1.toString()
                       << ", "
                       << instruction.trueLabel
                       << ", "
                       << instruction.falseLabel;
                break;
        }

        output << "\n";
    }

    output << "    Successors: ";

    for (size_t i = 0; i < successors.size(); ++i) {
        if (i > 0) {
            output << ", ";
        }

        output << "B" << successors[i]->id;
    }

    output << "\n";

    output << "    Predecessors: ";

    for (size_t i = 0; i < predecessors.size(); ++i) {
        if (i > 0) {
            output << ", ";
        }

        output << "B" << predecessors[i]->id;
    }

    output << "\n";

    return output.str();
}
