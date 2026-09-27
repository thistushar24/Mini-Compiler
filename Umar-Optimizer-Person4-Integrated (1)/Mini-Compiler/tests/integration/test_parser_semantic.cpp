#include "../../src/parser/Parser.h"
#include "../../src/semantic/SemanticAnalyzer.h"
#include <iostream>

using namespace parser;
using namespace semantic;

int main() {
    // We simulate Person 1's output for: int x = 10; x = x + 5;
    std::vector<Token> tokens = {
        Token(TokenType::LET, "int", 1, 1),
        Token(TokenType::IDENTIFIER, "x", 1, 5),
        Token(TokenType::ASSIGN, "=", 1, 7),
        Token(TokenType::INTEGER, "10", 1, 9),
        Token(TokenType::SEMICOLON, ";", 1, 11),
        
        Token(TokenType::IDENTIFIER, "x", 2, 1),
        Token(TokenType::ASSIGN, "=", 2, 3),
        Token(TokenType::IDENTIFIER, "x", 2, 5),
        Token(TokenType::PLUS, "+", 2, 7),
        Token(TokenType::INTEGER, "5", 2, 9),
        Token(TokenType::SEMICOLON, ";", 2, 10),
        
        Token(TokenType::END_OF_FILE, "", 3, 1)
    };

    std::cout << "Starting parser test...\n";
    
    Parser parser(tokens);
    auto ast = parser.parse();
    
    std::cout << "Parsed " << ast.size() << " statements.\n";

    std::cout << "Starting semantic analysis...\n";
    SemanticAnalyzer analyzer;
    analyzer.analyze(ast);

    if (analyzer.hasErrors()) {
        std::cout << "Semantic errors found!\n";
        for (const auto& err : analyzer.getErrors()) {
            std::cout << " - " << err << "\n";
        }
    } else {
        std::cout << "Semantic analysis passed!\n";
    }

    return 0;
}
