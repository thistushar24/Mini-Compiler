#include "Parser.h"
#include "../ast/BlockStatement.h"
#include "../ast/VariableDeclaration.h"
#include "../ast/IfStatement.h"
#include "../ast/WhileStatement.h"
#include "../ast/AssignmentStatement.h"
#include <stdexcept>
#include <iostream>

namespace parser {

Parser::Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

std::vector<std::unique_ptr<ast::Statement>> Parser::parse() {
    std::vector<std::unique_ptr<ast::Statement>> statements;
    while (!isAtEnd()) {
        try {
            statements.push_back(declaration());
        } catch (const std::runtime_error& e) {
            std::cerr << "Syntax Error: " << e.what() << std::endl;
            synchronize();
        }
    }
    return statements;
}

std::unique_ptr<ast::Statement> Parser::declaration() {
    if (match({TokenType::LET})) {
        return variableDeclaration();
    }
    return statement();
}

std::unique_ptr<ast::Statement> Parser::variableDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
    
    std::unique_ptr<ast::Expression> initializer = nullptr;
    if (match({TokenType::ASSIGN})) {
        initializer = expression();
    }
    
    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_unique<ast::VariableDeclaration>("int", name.lexeme, std::move(initializer));
}

std::unique_ptr<ast::Statement> Parser::statement() {
    if (match({TokenType::IF})) return ifStatement();
    if (match({TokenType::WHILE})) return whileStatement();
    if (match({TokenType::LBRACE})) return blockStatement();
    
    return expressionStatement();
}

std::unique_ptr<ast::Statement> Parser::ifStatement() {
    consume(TokenType::LPAREN, "Expect '(' after 'if'.");
    std::unique_ptr<ast::Expression> condition = expression();
    consume(TokenType::RPAREN, "Expect ')' after if condition.");
    
    std::unique_ptr<ast::Statement> thenBranch = statement();
    std::unique_ptr<ast::Statement> elseBranch = nullptr;
    
    if (match({TokenType::ELSE})) {
        elseBranch = statement();
    }
    
    return std::make_unique<ast::IfStatement>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

std::unique_ptr<ast::Statement> Parser::whileStatement() {
    consume(TokenType::LPAREN, "Expect '(' after 'while'.");
    std::unique_ptr<ast::Expression> condition = expression();
    consume(TokenType::RPAREN, "Expect ')' after condition.");
    
    std::unique_ptr<ast::Statement> body = statement();
    
    return std::make_unique<ast::WhileStatement>(std::move(condition), std::move(body));
}

std::unique_ptr<ast::Statement> Parser::blockStatement() {
    std::vector<std::unique_ptr<ast::Statement>> statements;
    
    while (!check(TokenType::RBRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }
    
    consume(TokenType::RBRACE, "Expect '}' after block.");
    return std::make_unique<ast::BlockStatement>(std::move(statements));
}

std::unique_ptr<ast::Statement> Parser::expressionStatement() {
    if (check(TokenType::IDENTIFIER) && current + 1 < tokens.size() && tokens[current + 1].type == TokenType::ASSIGN) {
        Token name = consume(TokenType::IDENTIFIER, "");
        consume(TokenType::ASSIGN, "");
        std::unique_ptr<ast::Expression> value = expression();
        consume(TokenType::SEMICOLON, "Expect ';' after assignment.");
        return std::make_unique<ast::AssignmentStatement>(name.lexeme, std::move(value));
    }
    
    // As per the AST design, we don't have an ExpressionStatement.
    throw std::runtime_error("Expression statements (other than assignments) not fully supported.");
}

// Helpers
bool Parser::isAtEnd() const {
    return peek().type == TokenType::END_OF_FILE;
}

const Token& Parser::peek() const {
    return tokens[current];
}

const Token& Parser::previous() const {
    return tokens[current - 1];
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(std::initializer_list<TokenType> types) {
    for (TokenType type : types) {
        if (check(type)) {
            current++;
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) {
        return tokens[current++];
    }
    throw std::runtime_error(message + " at line " + std::to_string(peek().line));
}

void Parser::synchronize() {
    current++;
    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON) return;
        switch (peek().type) {
            case TokenType::LET:
            case TokenType::IF:
            case TokenType::WHILE:
                return;
            default:
                break;
        }
        current++;
    }
}

} // namespace parser
