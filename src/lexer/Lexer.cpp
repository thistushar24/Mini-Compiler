// Lexer.cpp
// Implements the Lexer declared in Lexer.h, producing a token stream
// for MiniLang as specified in LANGUAGE_SPEC.md.

#include "Lexer.h"
#include <cctype>

// Maps every MiniLang keyword lexeme to its reserved TokenType.
// Any identifier not found here is treated as TokenType::IDENTIFIER.
const std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"let",   TokenType::LET},
    {"print", TokenType::PRINT},
    {"if",    TokenType::IF},
    {"else",  TokenType::ELSE},
    {"while", TokenType::WHILE}
};

Lexer::Lexer(std::string source)
    : source(std::move(source)),
      start(0),
      current(0),
      line(1),
      column(1),
      startColumn(1) {}

// ---------------------------------------------------------------------
// Character stream helpers
// ---------------------------------------------------------------------

bool Lexer::isAtEnd() const {
    return current >= source.size();
}

char Lexer::advance() {
    char c = source[current];
    current++;
    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return c;
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source[current];
}

char Lexer::peekNext() const {
    if (current + 1 >= source.size()) return '\0';
    return source[current + 1];
}

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;
    advance();
    return true;
}

// ---------------------------------------------------------------------
// Character classification
// ---------------------------------------------------------------------

bool Lexer::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

bool Lexer::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

// ---------------------------------------------------------------------
// Token emission
// ---------------------------------------------------------------------

void Lexer::addToken(std::vector<Token>& tokens, TokenType type) {
    std::string lexeme = source.substr(start, current - start);
    tokens.emplace_back(type, lexeme, line, startColumn);
}

// ---------------------------------------------------------------------
// Whitespace
// ---------------------------------------------------------------------

// MiniLang's grammar defines no comment syntax, so this only consumes
// whitespace (spaces, tabs, carriage returns, and newlines).
void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            advance();
        } else {
            break;
        }
    }
}

// ---------------------------------------------------------------------
// Identifiers and keywords
// ---------------------------------------------------------------------

// identifier → LETTER (LETTER | DIGIT | "_")*
// Keywords are matched by exact lexeme lookup after scanning the full
// identifier, so "let" becomes LET but "letter" stays IDENTIFIER.
void Lexer::identifier(std::vector<Token>& tokens) {
    while (isAlphaNumeric(peek())) {
        advance();
    }

    std::string text = source.substr(start, current - start);
    auto it = keywords.find(text);
    TokenType type = (it != keywords.end()) ? it->second : TokenType::IDENTIFIER;
    addToken(tokens, type);
}

// ---------------------------------------------------------------------
// Numbers
// ---------------------------------------------------------------------

// INTEGER → DIGIT+
// FLOAT   → DIGIT+ "." DIGIT+
//
// A '.' is only consumed as part of the number if it is followed by at
// least one digit; this keeps "10." from being misread and leaves any
// trailing '.' for a later token/UNKNOWN to handle.
void Lexer::number(std::vector<Token>& tokens) {
    while (isDigit(peek())) {
        advance();
    }

    bool isFloat = false;
    if (peek() == '.' && isDigit(peekNext())) {
        isFloat = true;
        advance(); // consume '.'
        while (isDigit(peek())) {
            advance();
        }
    }

    addToken(tokens, isFloat ? TokenType::FLOAT : TokenType::INTEGER);
}

// ---------------------------------------------------------------------
// Core scanning
// ---------------------------------------------------------------------

void Lexer::scanToken(std::vector<Token>& tokens) {
    char c = advance();

    switch (c) {
        // Single-character delimiters
        case '(': addToken(tokens, TokenType::LPAREN); break;
        case ')': addToken(tokens, TokenType::RPAREN); break;
        case '{': addToken(tokens, TokenType::LBRACE); break;
        case '}': addToken(tokens, TokenType::RBRACE); break;
        case ';': addToken(tokens, TokenType::SEMICOLON); break;

        // Single-character arithmetic operators
        case '+': addToken(tokens, TokenType::PLUS); break;
        case '-': addToken(tokens, TokenType::MINUS); break;
        case '*': addToken(tokens, TokenType::STAR); break;
        case '/': addToken(tokens, TokenType::SLASH); break;
        case '%': addToken(tokens, TokenType::PERCENT); break;

        // Operators that may be one or two characters
        case '=':
            addToken(tokens, match('=') ? TokenType::EQUAL_EQUAL
                                         : TokenType::ASSIGN);
            break;
        case '!':
            if (match('=')) {
                addToken(tokens, TokenType::BANG_EQUAL);
            } else {
                // '!' alone is not part of the MiniLang grammar.
                addToken(tokens, TokenType::UNKNOWN);
            }
            break;
        case '<':
            addToken(tokens, match('=') ? TokenType::LESS_EQUAL
                                         : TokenType::LESS);
            break;
        case '>':
            addToken(tokens, match('=') ? TokenType::GREATER_EQUAL
                                         : TokenType::GREATER);
            break;

        default:
            if (isDigit(c)) {
                number(tokens);
            } else if (isAlpha(c)) {
                identifier(tokens);
            } else {
                // Any character not defined by the MiniLang grammar
                // (e.g. '@', '#', '&', '"') becomes UNKNOWN so the
                // Parser/diagnostics layer can report it, rather than
                // the Lexer silently dropping or crashing on it.
                addToken(tokens, TokenType::UNKNOWN);
            }
            break;
    }
}

// ---------------------------------------------------------------------
// Public entry point
// ---------------------------------------------------------------------

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (true) {
        skipWhitespace();
        if (isAtEnd()) break;

        start = current;
        startColumn = column;
        scanToken(tokens);
    }

    // Emit a single trailing END_OF_FILE token at the final position.
    tokens.emplace_back(TokenType::END_OF_FILE, "", line, column);
    return tokens;
}
