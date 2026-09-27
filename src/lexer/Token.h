// Token.h
// Defines the Token structure produced by the Lexer and consumed by
// the Parser for the MiniLang mini compiler.
//
// This header only depends on TokenType.h and the standard library.
// It intentionally contains no lexing logic.

#pragma once

#include "TokenType.h"
#include <string>
#include <sstream>
#include <utility>

// Converts a TokenType to a human-readable name.
// Useful for diagnostics, debugging, and Token::toString().
inline std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::LET:            return "LET";
        case TokenType::PRINT:          return "PRINT";
        case TokenType::IF:             return "IF";
        case TokenType::ELSE:           return "ELSE";
        case TokenType::WHILE:          return "WHILE";

        case TokenType::IDENTIFIER:     return "IDENTIFIER";

        case TokenType::INTEGER:        return "INTEGER";
        case TokenType::FLOAT:          return "FLOAT";

        case TokenType::PLUS:           return "PLUS";
        case TokenType::MINUS:          return "MINUS";
        case TokenType::STAR:           return "STAR";
        case TokenType::SLASH:          return "SLASH";
        case TokenType::PERCENT:        return "PERCENT";

        case TokenType::EQUAL_EQUAL:    return "EQUAL_EQUAL";
        case TokenType::BANG_EQUAL:     return "BANG_EQUAL";
        case TokenType::LESS:           return "LESS";
        case TokenType::GREATER:        return "GREATER";
        case TokenType::LESS_EQUAL:     return "LESS_EQUAL";
        case TokenType::GREATER_EQUAL:  return "GREATER_EQUAL";

        case TokenType::ASSIGN:         return "ASSIGN";

        case TokenType::LPAREN:         return "LPAREN";
        case TokenType::RPAREN:         return "RPAREN";
        case TokenType::LBRACE:         return "LBRACE";
        case TokenType::RBRACE:         return "RBRACE";
        case TokenType::SEMICOLON:      return "SEMICOLON";

        case TokenType::END_OF_FILE:    return "END_OF_FILE";
        case TokenType::UNKNOWN:        return "UNKNOWN";

        default:                        return "INVALID_TOKEN_TYPE";
    }
}

// Represents a single lexical token: its type, the exact source text
// it came from (the lexeme), and its position for error reporting.
struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;

    // Default constructor: produces an UNKNOWN token at (line 1, column 1).
    Token()
        : type(TokenType::UNKNOWN), lexeme(""), line(1), column(1) {}

    Token(TokenType type, std::string lexeme, int line, int column)
        : type(type), lexeme(std::move(lexeme)), line(line), column(column) {}

    // Human-readable representation, e.g.:
    // Token(INTEGER, "25", line: 1, column: 10)
    std::string toString() const {
        std::ostringstream out;
        out << "Token(" << tokenTypeToString(type)
            << ", \"" << lexeme << "\""
            << ", line: " << line
            << ", column: " << column
            << ")";
        return out.str();
    }
};
