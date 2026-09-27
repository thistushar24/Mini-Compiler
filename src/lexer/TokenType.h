// TokenType.h
// Defines all lexical token categories for the MiniLang mini compiler.
//
// This header has no dependencies and is safe to include from the
// Lexer, Parser, and any AST/diagnostics code.

#pragma once

enum class TokenType {
    // ---- Keywords ----
    LET,
    PRINT,
    IF,
    ELSE,
    WHILE,

    // ---- Identifiers ----
    IDENTIFIER,

    // ---- Literals ----
    INTEGER,
    FLOAT,

    // ---- Arithmetic operators ----
    PLUS,      // +
    MINUS,     // -
    STAR,      // *
    SLASH,     // /
    PERCENT,   // %

    // ---- Comparison operators ----
    EQUAL_EQUAL,    // ==
    BANG_EQUAL,     // !=
    LESS,           // <
    GREATER,        // >
    LESS_EQUAL,     // <=
    GREATER_EQUAL,  // >=

    // ---- Assignment ----
    ASSIGN,    // =

    // ---- Delimiters ----
    LPAREN,     // (
    RPAREN,     // )
    LBRACE,     // {
    RBRACE,     // }
    SEMICOLON,  // ;

    // ---- Special ----
    END_OF_FILE,
    UNKNOWN
};
