#include "../../src/lexer/Lexer.h"
#include "../../src/parser/Parser.h"
#include "../../src/irgen/IRGenerator.h"
#include "../../src/codegen/CodeGenerator.h"

#include <iostream>
#include <string>
#include <vector>

int main() {
    const std::string source = R"(
        let x = 10;
        let y = 20;
        x = x + y;

        if (x > 20) {
            x = x - 1;
        }
    )";

    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    parser::Parser parser(tokens);
    auto statements = parser.parse();

    if (statements.empty()) {
        std::cerr << "ERROR: Parser produced no statements.\n";
        return 1;
    }

    IRGenerator irGenerator;
    IRProgram ir = irGenerator.generate(statements);

    if (ir.instructions.empty()) {
        std::cerr << "ERROR: IR is empty.\n";
        return 1;
    }

    CodeGenerator codeGenerator;
    std::string targetCode = codeGenerator.generate(ir);

    std::cout << "===== GENERATED TARGET CODE =====\n";
    std::cout << targetCode;
    std::cout << "=================================\n";

    if (targetCode.empty()) {
        std::cerr << "ERROR: Generated target code is empty.\n";
        return 1;
    }

    std::cout << "Code generation test passed.\n";

    return 0;
}
