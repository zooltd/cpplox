#include "Expr.h"

cpplox::BinaryExpr::BinaryExpr(pExpr left, Token op, pExpr right)
    : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

cpplox::UnaryExpr::UnaryExpr(Token op, pExpr right)
    : op(std::move(op)), right(std::move(right)) {}

cpplox::LiteralExpr::LiteralExpr(Object value) : value(std::move(value)) {}

cpplox::GroupingExpr::GroupingExpr(pExpr expression)
    : expression(std::move(expression)) {}
