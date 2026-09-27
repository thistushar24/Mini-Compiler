#include "Parser.h"
#include "../ast/BinaryExpression.h"
#include "../ast/LiteralExpression.h"
#include "../ast/VariableExpression.h"
#include "../ast/AssignmentStatement.h"
#include <stdexcept>

namespace parser {

// Since the prompt explicitly asked for AssignmentStatement but assignment is typically an expression,
// we will parse assignments at the statement level in Parser.cpp, but if we handle it here, 
// we'll need to adapt it. 
// For this structure, we'll implement standard expression parsing.

std::unique_ptr<ast::Expression> Parser::expression() {
    return assignment();
}

std::unique_ptr<ast::Expression> Parser::assignment() {
    // Normally assignment is an expression. 
    // If we want AssignmentStatement to be produced, it should be done in statement parsing.
    // For the sake of this AST, we will return an equality expression and let `expressionStatement` convert it 
    // if needed, or we just stick to equality for now.
    
    return equality();
}

std::unique_ptr<ast::Expression> Parser::equality() {
    std::unique_ptr<ast::Expression> expr = comparison();

    while (match({TokenType::EQUAL_EQUAL, TokenType::BANG_EQUAL})) {
        TokenType op = previous().type;
        std::unique_ptr<ast::Expression> right = comparison();
        expr = std::make_unique<ast::BinaryExpression>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<ast::Expression> Parser::comparison() {
    std::unique_ptr<ast::Expression> expr = term();

    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
        TokenType op = previous().type;
        std::unique_ptr<ast::Expression> right = term();
        expr = std::make_unique<ast::BinaryExpression>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<ast::Expression> Parser::term() {
    std::unique_ptr<ast::Expression> expr = factor();

    while (match({TokenType::PLUS, TokenType::MINUS})) {
        TokenType op = previous().type;
        std::unique_ptr<ast::Expression> right = factor();
        expr = std::make_unique<ast::BinaryExpression>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<ast::Expression> Parser::factor() {
    std::unique_ptr<ast::Expression> expr = primary();

    while (match({TokenType::STAR, TokenType::SLASH})) {
        TokenType op = previous().type;
        std::unique_ptr<ast::Expression> right = primary();
        expr = std::make_unique<ast::BinaryExpression>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<ast::Expression> Parser::primary() {
    if (match({TokenType::INTEGER, TokenType::FLOAT})) {
        return std::make_unique<ast::LiteralExpression>(previous().lexeme);
    }
    
    if (match({TokenType::IDENTIFIER})) {
        return std::make_unique<ast::VariableExpression>(previous().lexeme);
    }
    
    if (match({TokenType::LPAREN})) {
        std::unique_ptr<ast::Expression> expr = expression();
        consume(TokenType::RPAREN, "Expect ')' after expression.");
        return expr;
    }

    throw std::runtime_error("Expect expression.");
}

} // namespace parser
