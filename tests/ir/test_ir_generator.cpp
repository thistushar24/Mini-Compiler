#include "../../src/lexer/Lexer.h"
#include "../../src/parser/Parser.h"
#include "../../src/irgen/IRGenerator.h"

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

        while (x > 0) {
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

    IRGenerator generator;
    IRProgram program = generator.generate(statements);

    std::cout << "===== GENERATED IR =====\n";
    std::cout << program.toString();
    std::cout << "========================\n";

    if (program.instructions.empty()) {
        std::cerr << "ERROR: IR is empty.\n";
        return 1;
    }

    std::cout << "IR generation test passed.\n";

    return 0;
}
