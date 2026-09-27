#pragma once

#include <string>
#include <utility>

enum class IROperandType {
    CONSTANT,
    VARIABLE,
    TEMPORARY
};

struct IROperand {
    IROperandType type;
    std::string value;

    IROperand()
        : type(IROperandType::VARIABLE), value("") {}

    IROperand(IROperandType type, std::string value)
        : type(type), value(std::move(value)) {}

    static IROperand constant(const std::string& value) {
        return IROperand(IROperandType::CONSTANT, value);
    }

    static IROperand variable(const std::string& value) {
        return IROperand(IROperandType::VARIABLE, value);
    }

    static IROperand temporary(const std::string& value) {
        return IROperand(IROperandType::TEMPORARY, value);
    }

    std::string toString() const {
        return value;
    }
};
