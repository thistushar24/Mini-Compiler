#include "IRGenerator.h"

#include "../lexer/TokenType.h"

#include <cctype>
#include <stdexcept>

std::string IRGenerator::newTemporary() {
    return "t" + std::to_string(++temporaryCounter);
}

std::string IRGenerator::newLabel() {
    return "L" + std::to_string(++labelCounter);
}

IROperand IRGenerator::makeOperand(const std::string& value) const {
    if (value.size() >= 2 &&
        value[0] == 't' &&
        std::isdigit(static_cast<unsigned char>(value[1]))) {
        return IROperand::temporary(value);
    }

    bool numeric = !value.empty();
    bool decimal = false;

    for (char c : value) {
        if (c == '.') {
            if (decimal) {
                numeric = false;
                break;
            }
            decimal = true;
        }
        else if (!std::isdigit(static_cast<unsigned char>(c))) {
            numeric = false;
            break;
        }
    }

    if (numeric) {
        return IROperand::constant(value);
    }

    return IROperand::variable(value);
}

IRProgram IRGenerator::generate(
    const std::vector<std::unique_ptr<ast::Statement>>& statements
) {
    program.clear();

    temporaryCounter = 0;
    labelCounter = 0;
    lastValue.clear();

    for (const auto& statement : statements) {
        if (statement) {
            statement->accept(*this);
        }
    }

    return program;
}

std::string IRGenerator::generateExpression(ast::Expression& expression) {
    expression.accept(*this);
    return lastValue;
}

void IRGenerator::visit(ast::LiteralExpression& node) {
    lastValue = node.value;
}

void IRGenerator::visit(ast::VariableExpression& node) {
    lastValue = node.name;
}

void IRGenerator::visit(ast::BinaryExpression& node) {
    std::string left = generateExpression(*node.left);
    std::string right = generateExpression(*node.right);

    IROpcode opcode;

    switch (node.op) {
        case TokenType::PLUS:
            opcode = IROpcode::ADD;
            break;

        case TokenType::MINUS:
            opcode = IROpcode::SUB;
            break;

        case TokenType::STAR:
            opcode = IROpcode::MUL;
            break;

        case TokenType::SLASH:
            opcode = IROpcode::DIV;
            break;

        case TokenType::PERCENT:
            opcode = IROpcode::MOD;
            break;

        case TokenType::EQUAL_EQUAL:
            opcode = IROpcode::CMP_EQ;
            break;

        case TokenType::BANG_EQUAL:
            opcode = IROpcode::CMP_NE;
            break;

        case TokenType::LESS:
            opcode = IROpcode::CMP_LT;
            break;

        case TokenType::GREATER:
            opcode = IROpcode::CMP_GT;
            break;

        case TokenType::LESS_EQUAL:
            opcode = IROpcode::CMP_LE;
            break;

        case TokenType::GREATER_EQUAL:
            opcode = IROpcode::CMP_GE;
            break;

        default:
            throw std::runtime_error(
                "Unsupported binary operator in IR generation."
            );
    }

    std::string temporary = newTemporary();

    program.emit(
        IRInstruction::makeBinary(
            opcode,
            IROperand::temporary(temporary),
            makeOperand(left),
            makeOperand(right)
        )
    );

    lastValue = temporary;
}

void IRGenerator::visit(ast::VariableDeclaration& node) {
    if (!node.initializer) {
        return;
    }

    std::string value = generateExpression(*node.initializer);

    program.emit(
        IRInstruction::makeMov(
            IROperand::variable(node.variableName),
            makeOperand(value)
        )
    );
}

void IRGenerator::visit(ast::AssignmentStatement& node) {
    std::string value = generateExpression(*node.value);

    program.emit(
        IRInstruction::makeMov(
            IROperand::variable(node.variableName),
            makeOperand(value)
        )
    );
}

void IRGenerator::visit(ast::BlockStatement& node) {
    for (const auto& statement : node.statements) {
        if (statement) {
            statement->accept(*this);
        }
    }
}

void IRGenerator::visit(ast::IfStatement& node) {
    std::string condition = generateExpression(*node.condition);

    std::string thenLabel = newLabel();
    std::string elseLabel = newLabel();
    std::string endLabel = newLabel();

    program.emit(
        IRInstruction::makeBranch(
            makeOperand(condition),
            thenLabel,
            elseLabel
        )
    );

    program.emit(IRInstruction::makeLabel(thenLabel));

    if (node.thenBranch) {
        node.thenBranch->accept(*this);
    }

    program.emit(IRInstruction::makeJump(endLabel));

    program.emit(IRInstruction::makeLabel(elseLabel));

    if (node.elseBranch) {
        node.elseBranch->accept(*this);
    }

    program.emit(IRInstruction::makeJump(endLabel));

    program.emit(IRInstruction::makeLabel(endLabel));
}

void IRGenerator::visit(ast::WhileStatement& node) {
    std::string conditionLabel = newLabel();
    std::string bodyLabel = newLabel();
    std::string exitLabel = newLabel();

    program.emit(IRInstruction::makeLabel(conditionLabel));

    std::string condition = generateExpression(*node.condition);

    program.emit(
        IRInstruction::makeBranch(
            makeOperand(condition),
            bodyLabel,
            exitLabel
        )
    );

    program.emit(IRInstruction::makeLabel(bodyLabel));

    if (node.body) {
        node.body->accept(*this);
    }

    program.emit(IRInstruction::makeJump(conditionLabel));

    program.emit(IRInstruction::makeLabel(exitLabel));
}
