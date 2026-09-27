// Lexer.h
// Declares the Lexer for the MiniLang mini compiler.
//
// The Lexer converts MiniLang source code (as a std::string) into a
// flat sequence of Token values, terminated by an END_OF_FILE token.
// It depends only on Token.h / TokenType.h and the standard library.

#pragma once

#include "Token.h"
#include "TokenType.h"
#include <string>
#include <vector>
#include <unordered_map>

class Lexer {
public:
    // Constructs a Lexer over the given source code.
    explicit Lexer(std::string source);

    // Scans the entire source and returns the full token stream,
    // ending with a single END_OF_FILE token.
    std::vector<Token> tokenize();

private:
    // ---- Source state ----
    std::string source;
    size_t start;    // index of the first character of the token being scanned
    size_t current;  // index of the character currently being examined
    int line;        // current line number (1-based)
    int column;      // column of 'current' on the current line (1-based)
    int startColumn; // column where the current token began

    // Keyword lookup: lexeme -> TokenType
    static const std::unordered_map<std::string, TokenType> keywords;

    // ---- Core scanning ----
    void scanToken(std::vector<Token>& tokens);
    void identifier(std::vector<Token>& tokens);
    void number(std::vector<Token>& tokens);
    void skipWhitespace();

    // ---- Character stream helpers ----
    bool isAtEnd() const;
    char advance();
    char peek() const;
    char peekNext() const;
    bool match(char expected);

    // ---- Character classification ----
    static bool isDigit(char c);
    static bool isAlpha(char c);
    static bool isAlphaNumeric(char c);

    // ---- Token emission ----
    void addToken(std::vector<Token>& tokens, TokenType type);
};
