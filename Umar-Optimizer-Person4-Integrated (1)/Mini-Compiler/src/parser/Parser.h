#pragma once
#include <vector>
#include <memory>
#include <initializer_list>
#include <string>
#include "../lexer/Token.h"
#include "../ast/Statement.h"
#include "../ast/Expression.h"

namespace parser {

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    
    // Main entry point. Returns a list of statements (the program)
    std::vector<std::unique_ptr<ast::Statement>> parse();

private:
    std::vector<Token> tokens;
    size_t current = 0;

    // --- Helper Methods ---
    bool isAtEnd() const;
    const Token& peek() const;
    const Token& previous() const;
    bool check(TokenType type) const;
    bool match(std::initializer_list<TokenType> types);
    Token consume(TokenType type, const std::string& message);

    // --- Statement Parsing (implemented in Parser.cpp) ---
    std::unique_ptr<ast::Statement> declaration();
    std::unique_ptr<ast::Statement> variableDeclaration();
    std::unique_ptr<ast::Statement> statement();
    std::unique_ptr<ast::Statement> ifStatement();
    std::unique_ptr<ast::Statement> whileStatement();
    std::unique_ptr<ast::Statement> blockStatement();
    std::unique_ptr<ast::Statement> expressionStatement();

    // --- Expression Parsing (implemented in ExpressionParser.cpp) ---
    std::unique_ptr<ast::Expression> expression();
    std::unique_ptr<ast::Expression> assignment();
    std::unique_ptr<ast::Expression> equality();
    std::unique_ptr<ast::Expression> comparison();
    std::unique_ptr<ast::Expression> term();
    std::unique_ptr<ast::Expression> factor();
    std::unique_ptr<ast::Expression> primary();
    
    // Error handling
    void synchronize();
};

} // namespace parser
