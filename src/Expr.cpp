#include "Expr.h"

cpplox::AST::AssignExpr::AssignExpr(Token name, pExpr value)
    : name(std::move(name)), value(std::move(value)) {}

cpplox::AST::BinaryExpr::BinaryExpr(pExpr left, Token op, pExpr right)
    : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

cpplox::AST::CallExpr::CallExpr(pExpr callee, Token paren, std::vector<pExpr> arguments)
    : callee(std::move(callee)), paren(std::move(paren)), arguments(std::move(arguments)) {}

cpplox::AST::UnaryExpr::UnaryExpr(Token op, pExpr right)
    : op(std::move(op)), right(std::move(right)) {}

cpplox::AST::LiteralExpr::LiteralExpr(Object value)
    : value(std::move(value)) {}

cpplox::AST::LogicalExpr::LogicalExpr(pExpr left, Token op, pExpr right)
    : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

cpplox::AST::GroupingExpr::GroupingExpr(pExpr expression)
    : expression(std::move(expression)) {}

cpplox::AST::VariableExpr::VariableExpr(Token name)
    : name(std::move(name)) {}
