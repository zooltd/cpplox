#include "Stmt.h"

#include <utility>

cpplox::AST::ExpressionStmt::ExpressionStmt(cpplox::AST::pExpr expression)
    : expression(std::move(expression)) {}

cpplox::AST::PrintStmt::PrintStmt(cpplox::AST::pExpr expression)
    : expression(std::move(expression)) {}

cpplox::AST::VarStmt::VarStmt(cpplox::Token name, cpplox::AST::pExpr initializer)
    : name(std::move(name)), initializer(std::move(initializer)) {}
