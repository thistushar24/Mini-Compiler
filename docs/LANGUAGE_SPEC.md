# MiniLang Language Specification

This document defines the grammar for **MiniLang**, the source language
consumed by our Mini Compiler + Optimization Engine.

It is the contract between:
- the **Lexer** (produces tokens defined in `TokenType.h` / `Token.h`)
- the **Parser** (builds an AST according to the grammar below)

---

## 1. Overview

MiniLang is a small, statically-scoped imperative language supporting:

- Variable declaration (`let`)
- Assignment
- Arithmetic expressions (`+ - * / %`)
- Comparison expressions (`== != < > <= >=`)
- `print` statements
- `if` / `else` conditionals
- `while` loops
- Integer and floating-point literals

Example program:

```
let x = 10;
let total = 0;

while (x > 0) {
    total = total + x;
    x = x - 1;
}

if (total >= 50) {
    print(total);
} else {
    print(0);
}
```

---

## 2. Lexical Tokens

Every token produced by the Lexer must map to one value of the
`TokenType` enum (see `TokenType.h`).

| Category    | Tokens                                                              |
|-------------|----------------------------------------------------------------------|
| Keywords    | `LET`, `PRINT`, `IF`, `ELSE`, `WHILE`                                 |
| Identifier  | `IDENTIFIER`                                                          |
| Literals    | `INTEGER`, `FLOAT`                                                    |
| Arithmetic  | `PLUS`, `MINUS`, `STAR`, `SLASH`, `PERCENT`                           |
| Comparison  | `EQUAL_EQUAL`, `BANG_EQUAL`, `LESS`, `GREATER`, `LESS_EQUAL`, `GREATER_EQUAL` |
| Assignment  | `ASSIGN`                                                               |
| Delimiters  | `LPAREN`, `RPAREN`, `LBRACE`, `RBRACE`, `SEMICOLON`                    |
| Special     | `END_OF_FILE`, `UNKNOWN`                                              |

Notes:
- Keywords are reserved words and take priority over `IDENTIFIER` when
  matched exactly (e.g. `let` is always `LET`, never an identifier).
- `UNKNOWN` is reserved for any character or sequence the Lexer cannot
  classify (used for error reporting, not part of valid programs).

---

## 3. Grammar (EBNF)

Notation:
- `→` defines a rule
- `|` denotes alternatives
- `*` means zero or more repetitions
- `?` means optional (zero or one)
- Terminals are `UPPERCASE` (token types) or literal characters in quotes

### 3.1 Program

```
program
    → statement* END_OF_FILE
```

### 3.2 Statements

```
statement
    → varDecl
    | assignment
    | printStmt
    | ifStmt
    | whileStmt
    | block

varDecl
    → "let" IDENTIFIER "=" expression ";"

assignment
    → IDENTIFIER "=" expression ";"

printStmt
    → "print" "(" expression ")" ";"

ifStmt
    → "if" "(" expression ")" block ( "else" block )?

whileStmt
    → "while" "(" expression ")" block

block
    → "{" statement* "}"
```

### 3.3 Expressions (precedence, lowest to highest)

```
expression
    → equality

equality
    → comparison ( ( "==" | "!=" ) comparison )*

comparison
    → term ( ( "<" | ">" | "<=" | ">=" ) term )*

term
    → factor ( ( "+" | "-" ) factor )*

factor
    → unary ( ( "*" | "/" | "%" ) unary )*

unary
    → ( "+" | "-" ) unary
    | primary

primary
    → INTEGER
    | FLOAT
    | IDENTIFIER
    | "(" expression ")"
```

### 3.4 Lexical Rules

```
IDENTIFIER
    → LETTER ( LETTER | DIGIT | "_" )*

INTEGER
    → DIGIT+

FLOAT
    → DIGIT+ "." DIGIT+

LETTER
    → "a".."z" | "A".."Z" | "_"

DIGIT
    → "0".."9"
```

---

## 4. Operator Precedence Summary

From lowest to highest binding precedence:

1. Equality: `==` `!=`
2. Comparison: `<` `>` `<=` `>=`
3. Addition / Subtraction: `+` `-`
4. Multiplication / Division / Modulo: `*` `/` `%`
5. Unary: `+x` `-x`
6. Primary: literals, identifiers, parenthesized expressions

All binary operators in this grammar are **left-associative**.
Unary operators are **right-associative** (they bind to the expression
immediately to their right).

---

## 5. Token-to-Grammar Coverage

Every terminal used in the grammar above has a corresponding
`TokenType` value, and every `TokenType` value (aside from
`END_OF_FILE` and `UNKNOWN`, which are structural/error tokens rather
than grammar terminals) is used somewhere in the grammar:

| Grammar terminal | TokenType        |
|-------------------|------------------|
| `let`             | `LET`            |
| `print`           | `PRINT`          |
| `if`              | `IF`             |
| `else`            | `ELSE`           |
| `while`           | `WHILE`          |
| identifier        | `IDENTIFIER`     |
| integer literal   | `INTEGER`        |
| float literal     | `FLOAT`          |
| `+`               | `PLUS`           |
| `-`               | `MINUS`          |
| `*`               | `STAR`           |
| `/`               | `SLASH`          |
| `%`               | `PERCENT`        |
| `==`              | `EQUAL_EQUAL`    |
| `!=`              | `BANG_EQUAL`     |
| `<`               | `LESS`           |
| `>`               | `GREATER`        |
| `<=`              | `LESS_EQUAL`     |
| `>=`              | `GREATER_EQUAL`  |
| `=`               | `ASSIGN`         |
| `(`               | `LPAREN`         |
| `)`               | `RPAREN`         |
| `{`               | `LBRACE`         |
| `}`               | `RBRACE`         |
| `;`               | `SEMICOLON`      |
| end of input       | `END_OF_FILE`   |
| unrecognized text  | `UNKNOWN`       |

No token type is left unused, and no grammar rule relies on a token
type that isn't defined.

---

## 6. Design Notes for the Lexer/Parser Team

- `Token.h` provides `line` and `column` fields for every token so the
  Parser (and later stages) can produce precise error messages.
- `Token::toString()` is available for debugging Lexer/Parser output
  without needing a separate pretty-printer.
- The grammar is a standard recursive-descent-friendly design
  (each precedence level is its own rule), so the Parser can be
  implemented as one function per non-terminal without modification
  to these files.
- No new token types should be required for the currently specified
  language features (variables, arithmetic, comparisons, print,
  if/else, while). If a later feature needs a new operator or keyword,
  extend `TokenType.h` rather than overloading an existing token type.
