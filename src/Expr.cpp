#include "Expr.h"

cpplox::AST::BinaryExpr::BinaryExpr(pExpr left, Token op, pExpr right)
    : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

cpplox::AST::UnaryExpr::UnaryExpr(Token op, pExpr right)
    : op(std::move(op)), right(std::move(right)) {}

cpplox::AST::LiteralExpr::LiteralExpr(Object value) : value(std::move(value)) {}

cpplox::AST::GroupingExpr::GroupingExpr(pExpr expression)
    : expression(std::move(expression)) {}
