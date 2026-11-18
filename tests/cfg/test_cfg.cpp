#include "../../src/lexer/Lexer.h"
#include "../../src/parser/Parser.h"
#include "../../src/irgen/IRGenerator.h"
#include "../../src/cfg/ControlFlowGraph.h"

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
    IRProgram ir = generator.generate(statements);

    if (ir.instructions.empty()) {
        std::cerr << "ERROR: IR generation produced no instructions.\n";
        return 1;
    }

    ControlFlowGraph cfg;
    cfg.build(ir);

    const auto& blocks = cfg.getBlocks();

    std::cout << cfg.toString();

    if (blocks.empty()) {
        std::cerr << "ERROR: CFG contains no basic blocks.\n";
        return 1;
    }

    if (cfg.getEntryBlock() == nullptr) {
        std::cerr << "ERROR: CFG has no entry block.\n";
        return 1;
    }

    std::cout << "CFG block count: " << blocks.size() << "\n";
    std::cout << "CFG test passed.\n";

    return 0;
}
