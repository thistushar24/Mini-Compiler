#pragma once

#include "../ir/IRProgram.h"

#include <string>
#include <unordered_map>

class CodeGenerator {
public:
    std::string generate(const IRProgram& program);

private:
    std::unordered_map<std::string, std::string> registerMap;
    int nextRegister = 0;

    std::string getRegister(const IROperand& operand);
    std::string allocateRegister(const std::string& name);

    std::string opcodeToAssembly(
        IROpcode opcode
    ) const;
};
