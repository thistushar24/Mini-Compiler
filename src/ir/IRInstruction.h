#pragma once

#include "IROperand.h"

#include <string>
#include <vector>

enum class IROpcode {
    MOV,

    ADD,
    SUB,
    MUL,
    DIV,
    MOD,

    CMP_EQ,
    CMP_NE,
    CMP_LT,
    CMP_GT,
    CMP_LE,
    CMP_GE,

    NEG,

    LABEL,
    JUMP,
    BRANCH
};

struct IRInstruction {
    IROpcode opcode;

    IROperand result;
    IROperand operand1;
    IROperand operand2;

    std::string label;
    std::string trueLabel;
    std::string falseLabel;

    static IRInstruction makeMov(
        IROperand destination,
        IROperand source
    ) {
        IRInstruction instruction;
        instruction.opcode = IROpcode::MOV;
        instruction.result = std::move(destination);
        instruction.operand1 = std::move(source);
        return instruction;
    }

    static IRInstruction makeBinary(
        IROpcode opcode,
        IROperand result,
        IROperand left,
        IROperand right
    ) {
        IRInstruction instruction;
        instruction.opcode = opcode;
        instruction.result = std::move(result);
        instruction.operand1 = std::move(left);
        instruction.operand2 = std::move(right);
        return instruction;
    }

    static IRInstruction makeLabel(const std::string& label) {
        IRInstruction instruction;
        instruction.opcode = IROpcode::LABEL;
        instruction.label = label;
        return instruction;
    }

    static IRInstruction makeJump(const std::string& label) {
        IRInstruction instruction;
        instruction.opcode = IROpcode::JUMP;
        instruction.label = label;
        return instruction;
    }

    static IRInstruction makeBranch(
        IROperand condition,
        const std::string& trueLabel,
        const std::string& falseLabel
    ) {
        IRInstruction instruction;
        instruction.opcode = IROpcode::BRANCH;
        instruction.operand1 = std::move(condition);
        instruction.trueLabel = trueLabel;
        instruction.falseLabel = falseLabel;
        return instruction;
    }
};
