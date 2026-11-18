// main.cpp  — Mini Compiler Demo
// Runs the full pipeline on a sample MiniLang program and shows each stage.
//
// Pipeline:
//   Source → Lexer → Parser → Semantic Analyzer
//          → IR Generator → IR Optimizer → CFG → Code Generator

#include <iostream>
#include <string>
#include <vector>

#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "semantic/SemanticAnalyzer.h"
#include "irgen/IRGenerator.h"
#include "optimizer/IROptimizer.h"
#include "cfg/ControlFlowGraph.h"
#include "codegen/CodeGenerator.h"

static void printBanner(const std::string& title) {
    const int width = 50;
    std::string bar(width, '=');
    std::cout << "\n" << bar << "\n";
    int pad = (width - static_cast<int>(title.size())) / 2;
    if (pad > 0) std::cout << std::string(pad, ' ');
    std::cout << title << "\n";
    std::cout << bar << "\n";
}

int main() {
    // ----------------------------------------------------------------
    // Sample MiniLang source program
    // ----------------------------------------------------------------
    const std::string source = R"(
        let x = 10;
        let y = 20;
        let result = 0;

        x = x + y;

        if (x > 20) {
            result = x * 2;
        } else {
            result = x + 1;
        }

        let counter = 5;
        while (counter > 0) {
            result = result + counter;
            counter = counter - 1;
        }
    )";

    std::cout << "Mini Compiler — Full Pipeline Demo\n";
    printBanner("SOURCE CODE");
    std::cout << source << "\n";

    // ----------------------------------------------------------------
    // Stage 1: Lexical Analysis
    // ----------------------------------------------------------------
    printBanner("STAGE 1: LEXER");
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    std::cout << "Tokens produced: " << tokens.size() << "\n\n";
    for (const auto& tok : tokens) {
        std::cout << "  " << tok.toString() << "\n";
    }

    // ----------------------------------------------------------------
    // Stage 2: Parsing
    // ----------------------------------------------------------------
    printBanner("STAGE 2: PARSER");
    parser::Parser parser(tokens);
    auto ast = parser.parse();

    if (ast.empty()) {
        std::cerr << "ERROR: Parser produced no AST nodes. Aborting.\n";
        return 1;
    }
    std::cout << "AST statements parsed: " << ast.size() << "\n";
    std::cout << "Parser completed successfully.\n";

    // ----------------------------------------------------------------
    // Stage 3: Semantic Analysis
    // ----------------------------------------------------------------
    printBanner("STAGE 3: SEMANTIC ANALYZER");
    semantic::SemanticAnalyzer analyzer;
    analyzer.analyze(ast);

    if (analyzer.hasErrors()) {
        std::cout << "Semantic errors found:\n";
        for (const auto& err : analyzer.getErrors()) {
            std::cout << "  [ERROR] " << err << "\n";
        }
        return 1;
    }
    std::cout << "Semantic analysis passed — no errors.\n";

    // ----------------------------------------------------------------
    // Stage 4: IR Generation
    // ----------------------------------------------------------------
    printBanner("STAGE 4: IR GENERATION");
    IRGenerator irGenerator;
    IRProgram ir = irGenerator.generate(ast);

    if (ir.instructions.empty()) {
        std::cerr << "ERROR: IR is empty. Aborting.\n";
        return 1;
    }
    std::cout << ir.toString();
    std::cout << "IR instructions emitted: " << ir.instructions.size() << "\n";

    // ----------------------------------------------------------------
    // Stage 5: IR Optimization
    // ----------------------------------------------------------------
    printBanner("STAGE 5: IR OPTIMIZER");
    IROptimizer optimizer;
    IRProgram optimizedIR = optimizer.optimize(ir);

    std::cout << "Optimization pass complete.\n";
    std::cout << "Instructions before: " << ir.instructions.size()
              << "  |  after: " << optimizedIR.instructions.size() << "\n";

    // ----------------------------------------------------------------
    // Stage 6: Control Flow Graph
    // ----------------------------------------------------------------
    printBanner("STAGE 6: CONTROL FLOW GRAPH");
    ControlFlowGraph cfg;
    cfg.build(optimizedIR);

    const auto& blocks = cfg.getBlocks();
    if (blocks.empty()) {
        std::cerr << "ERROR: CFG has no basic blocks. Aborting.\n";
        return 1;
    }

    std::cout << cfg.toString();
    std::cout << "Basic blocks in CFG: " << blocks.size() << "\n";

    // ----------------------------------------------------------------
    // Stage 7: Code Generation
    // ----------------------------------------------------------------
    printBanner("STAGE 7: CODE GENERATION");
    CodeGenerator codeGen;
    std::string assembly = codeGen.generate(optimizedIR);

    if (assembly.empty()) {
        std::cerr << "ERROR: Code generator produced no output. Aborting.\n";
        return 1;
    }
    std::cout << assembly;

    // ----------------------------------------------------------------
    // Summary
    // ----------------------------------------------------------------
    printBanner("COMPILATION COMPLETE");
    std::cout << "  Tokens   : " << tokens.size()               << "\n";
    std::cout << "  AST nodes: " << ast.size()                  << "\n";
    std::cout << "  IR instrs: " << optimizedIR.instructions.size() << "\n";
    std::cout << "  CFG blocks: " << blocks.size()              << "\n";
    std::cout << "\nAll stages passed successfully.\n\n";

    return 0;
}
