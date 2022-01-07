#include "Interpreter.h"
#include "Meta.h"
#include <iostream>

void cpplox::Interpreter::interpret(const std::vector<AST::pStmt> &statements) {
    try {
        for (const AST::pStmt &pStmt: statements)
            execute(pStmt);
    } catch (const InterpretErr &error) {
        Errors::hadRuntimeError = true;
        logger::error(error);
    }
}

void cpplox::Interpreter::execute(const AST::pStmt &pStmt) {
    return std::visit(
            [this](auto &&pStmt) {
                using T = std::decay_t<decltype(pStmt)>;
                if constexpr (std::is_same_v<T, AST::pExpressionStmt>)
                    return evalExpressionStmt(pStmt);
                if constexpr (std::is_same_v<T, AST::pPrintStmt>)
                    return evalPrintStmt(pStmt);
            },
            pStmt);
}

cpplox::Object cpplox::Interpreter::evaluate(const AST::pExpr &pExpr) {
    return std::visit(
            [this](auto &&pExpr) {
                using T = std::decay_t<decltype(pExpr)>;
                if constexpr (std::is_same_v<T, AST::pLiteralExpr>)
                    return evalLiteralExpr(pExpr);
                else if constexpr (std::is_same_v<T, AST::pGroupingExpr>)
                    return evalGroupingExpr(pExpr);
                else if constexpr (std::is_same_v<T, AST::pUnaryExpr>)
                    return evalUnaryExpr(pExpr);
                else if constexpr (std::is_same_v<T, AST::pBinaryExpr>)
                    return evalBinaryExpr(pExpr);
                else
                    return std::monostate{};
            },
            pExpr);
}

void cpplox::Interpreter::evalExpressionStmt(const cpplox::AST::pExpressionStmt &pStmt) {
    evaluate(pStmt->expression);
}

void cpplox::Interpreter::evalPrintStmt(const cpplox::AST::pPrintStmt &pStmt) {
    Object value = evaluate(pStmt->expression);
    std::cout << value << std::endl;
}

cpplox::Object cpplox::Interpreter::evalLiteralExpr(const AST::pLiteralExpr &pExpr) {
    return pExpr->value;
}

cpplox::Object cpplox::Interpreter::evalGroupingExpr(const AST::pGroupingExpr &pExpr) {
    return evaluate(pExpr->expression);
}

cpplox::Object cpplox::Interpreter::evalUnaryExpr(const AST::pUnaryExpr &pExpr) {
    const Object right = evaluate(pExpr->right);
    switch (pExpr->op.type) {
        case TokenType::BANG:
            return !isTruthy(right);
        case TokenType::MINUS:
            checkNumberOperand(pExpr->op, right);
            return -std::get<double>(right);
        // Unreachable
        default:
            return std::monostate{};
    }
}

cpplox::Object cpplox::Interpreter::evalBinaryExpr(const AST::pBinaryExpr &pExpr) {
    Object left = evaluate(pExpr->left);
    Object right = evaluate(pExpr->right);
    switch (pExpr->op.type) {
        case TokenType::EQUAL_EQUAL:
            return left == right;
        case TokenType::BANG_EQUAL:
            return left != right;
        case TokenType::GREATER:
            checkNumberOperands(pExpr->op, left, right);
            return std::get<double>(left) > std::get<double>(right);
        case TokenType::GREATER_EQUAL:
            checkNumberOperands(pExpr->op, left, right);
            return std::get<double>(left) >= std::get<double>(right);
        case TokenType::LESS:
            checkNumberOperands(pExpr->op, left, right);
            return std::get<double>(left) < std::get<double>(right);
        case TokenType::LESS_EQUAL:
            checkNumberOperands(pExpr->op, left, right);
            return std::get<double>(left) <= std::get<double>(right);
        case TokenType::MINUS:
            checkNumberOperands(pExpr->op, left, right);
            return std::get<double>(left) - std::get<double>(right);
        case TokenType::SLASH:
            checkNumberOperands(pExpr->op, left, right);
            return std::get<double>(left) / std::get<double>(right);
        case TokenType::STAR:
            checkNumberOperands(pExpr->op, left, right);
            return std::get<double>(left) * std::get<double>(right);
        case TokenType::PLUS:
            if (std::holds_alternative<double>(left) &&
                std::holds_alternative<double>(right))
                return std::get<double>(left) + std::get<double>(right);
            if (std::holds_alternative<std::string>(left) &&
                std::holds_alternative<std::string>(right))
                return std::get<std::string>(left) + std::get<std::string>(right);
            throw InterpretErr(Meta::sourceFile, pExpr->op.line,
                               "Operands must be two numbers or two strings.");

        // Unreachable.
        default:
            return std::monostate{};
    }
}

bool cpplox::Interpreter::isTruthy(const Object &obj) const {
    if (std::holds_alternative<std::monostate>(obj))
        return false;
    if (std::holds_alternative<bool>(obj)) {
        return std::get<bool>(obj);
    }
    return true;
}

void cpplox::Interpreter::checkNumberOperand(const Token &op,
                                             const Object &operand) {
    if (std::holds_alternative<double>(operand))
        return;
    throw InterpretErr(Meta::sourceFile, op.line, "Operand must be a number.");
}

void cpplox::Interpreter::checkNumberOperands(const Token &op,
                                              const Object &left,
                                              const Object &right) {
    if (std::holds_alternative<double>(left) &&
        std::holds_alternative<double>(right))
        return;
    throw InterpretErr(Meta::sourceFile, op.line, "Operands must be numbers.");
}
