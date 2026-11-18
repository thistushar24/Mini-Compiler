#include "CodeGenerator.h"

#include <sstream>
#include <stdexcept>

std::string CodeGenerator::allocateRegister(
    const std::string& name
) {
    auto it = registerMap.find(name);

    if (it != registerMap.end()) {
        return it->second;
    }

    std::string reg = "R" + std::to_string(nextRegister++);

    registerMap[name] = reg;

    return reg;
}

std::string CodeGenerator::getRegister(
    const IROperand& operand
) {
    if (operand.type == IROperandType::CONSTANT) {
        return operand.value;
    }

    return allocateRegister(operand.value);
}

std::string CodeGenerator::opcodeToAssembly(
    IROpcode opcode
) const {
    switch (opcode) {
        case IROpcode::ADD:
            return "ADD";

        case IROpcode::SUB:
            return "SUB";

        case IROpcode::MUL:
            return "MUL";

        case IROpcode::DIV:
            return "DIV";

        case IROpcode::MOD:
            return "MOD";

        case IROpcode::CMP_EQ:
            return "CMPEQ";

        case IROpcode::CMP_NE:
            return "CMPNE";

        case IROpcode::CMP_LT:
            return "CMPLT";

        case IROpcode::CMP_GT:
            return "CMPGT";

        case IROpcode::CMP_LE:
            return "CMPLE";

        case IROpcode::CMP_GE:
            return "CMPGE";

        case IROpcode::NEG:
            return "NEG";

        default:
            return "";
    }
}

std::string CodeGenerator::generate(
    const IRProgram& program
) {
    registerMap.clear();
    nextRegister = 0;

    std::ostringstream output;

    for (const auto& instruction : program.instructions) {

        switch (instruction.opcode) {

            case IROpcode::MOV: {
                std::string destination =
                    getRegister(instruction.result);

                std::string source =
                    getRegister(instruction.operand1);

                output << "MOV "
                       << destination
                       << ", "
                       << source
                       << "\n";

                break;
            }

            case IROpcode::ADD:
            case IROpcode::SUB:
            case IROpcode::MUL:
            case IROpcode::DIV:
            case IROpcode::MOD:
            case IROpcode::CMP_EQ:
            case IROpcode::CMP_NE:
            case IROpcode::CMP_LT:
            case IROpcode::CMP_GT:
            case IROpcode::CMP_LE:
            case IROpcode::CMP_GE: {

                std::string destination =
                    getRegister(instruction.result);

                std::string left =
                    getRegister(instruction.operand1);

                std::string right =
                    getRegister(instruction.operand2);

                output << opcodeToAssembly(instruction.opcode)
                       << " "
                       << destination
                       << ", "
                       << left
                       << ", "
                       << right
                       << "\n";

                break;
            }

            case IROpcode::NEG: {

                std::string destination =
                    getRegister(instruction.result);

                std::string operand =
                    getRegister(instruction.operand1);

                output << "NEG "
                       << destination
                       << ", "
                       << operand
                       << "\n";

                break;
            }

            case IROpcode::LABEL:

                output << instruction.label
                       << ":\n";

                break;

            case IROpcode::JUMP:

                output << "JMP "
                       << instruction.label
                       << "\n";

                break;

            case IROpcode::BRANCH: {

                std::string condition =
                    getRegister(instruction.operand1);

                output << "BRANCH "
                       << condition
                       << ", "
                       << instruction.trueLabel
                       << ", "
                       << instruction.falseLabel
                       << "\n";

                break;
            }
        }
    }

    return output.str();
}
