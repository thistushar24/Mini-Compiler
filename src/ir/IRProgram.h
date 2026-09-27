#pragma once

#include "IRInstruction.h"

#include <vector>
#include <string>
#include <sstream>

class IRProgram {
public:
    std::vector<IRInstruction> instructions;

    void emit(const IRInstruction& instruction) {
        instructions.push_back(instruction);
    }

    void emit(IRInstruction&& instruction) {
        instructions.push_back(std::move(instruction));
    }

    void clear() {
        instructions.clear();
    }

    std::string toString() const {
        std::ostringstream output;

        for (const auto& instruction : instructions) {
            switch (instruction.opcode) {

                case IROpcode::MOV:
                    output << instruction.result.toString()
                           << " = "
                           << instruction.operand1.toString()
                           << "\n";
                    break;

                case IROpcode::ADD:
                    output << instruction.result.toString()
                           << " = "
                           << instruction.operand1.toString()
                           << " + "
                           << instruction.operand2.toString()
                           << "\n";
                    break;

                case IROpcode::SUB:
                    output << instruction.result.toString()
                           << " = "
                           << instruction.operand1.toString()
                           << " - "
                           << instruction.operand2.toString()
                           << "\n";
                    break;

                case IROpcode::MUL:
                    output << instruction.result.toString()
                           << " = "
                           << instruction.operand1.toString()
                           << " * "
                           << instruction.operand2.toString()
                           << "\n";
                    break;

                case IROpcode::DIV:
                    output << instruction.result.toString()
                           << " = "
                           << instruction.operand1.toString()
                           << " / "
                           << instruction.operand2.toString()
                           << "\n";
                    break;

                case IROpcode::MOD:
                    output << instruction.result.toString()
                           << " = "
                           << instruction.operand1.toString()
                           << " % "
                           << instruction.operand2.toString()
                           << "\n";
                    break;

                case IROpcode::CMP_EQ:
                    output << instruction.result.toString()
                           << " = "
                           << instruction.operand1.toString()
                           << " == "
                           << instruction.operand2.toString()
                           << "\n";
                    break;

                case IROpcode::CMP_NE:
                    output << instruction.result.toString()
                           << " = "
                           << instruction.operand1.toString()
                           << " != "
                           << instruction.operand2.toString()
                           << "\n";
                    break;

                case IROpcode::CMP_LT:
                    output << instruction.result.toString()
                           << " = "
                           << instruction.operand1.toString()
                           << " < "
                           << instruction.operand2.toString()
                           << "\n";
                    break;

                case IROpcode::CMP_GT:
                    output << instruction.result.toString()
                           << " = "
                           << instruction.operand1.toString()
                           << " > "
                           << instruction.operand2.toString()
                           << "\n";
                    break;

                case IROpcode::CMP_LE:
                    output << instruction.result.toString()
                           << " = "
                           << instruction.operand1.toString()
                           << " <= "
                           << instruction.operand2.toString()
                           << "\n";
                    break;

                case IROpcode::CMP_GE:
                    output << instruction.result.toString()
                           << " = "
                           << instruction.operand1.toString()
                           << " >= "
                           << instruction.operand2.toString()
                           << "\n";
                    break;

                case IROpcode::NEG:
                    output << instruction.result.toString()
                           << " = -"
                           << instruction.operand1.toString()
                           << "\n";
                    break;

                case IROpcode::LABEL:
                    output << instruction.label << ":\n";
                    break;

                case IROpcode::JUMP:
                    output << "JUMP " << instruction.label << "\n";
                    break;

                case IROpcode::BRANCH:
                    output << "BRANCH "
                           << instruction.operand1.toString()
                           << ", "
                           << instruction.trueLabel
                           << ", "
                           << instruction.falseLabel
                           << "\n";
                    break;
            }
        }

        return output.str();
    }
};
