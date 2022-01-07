#include "Stmt.h"

cpplox::AST::ExpressionStmt::ExpressionStmt(cpplox::AST::pExpr expression) : expression(std::move(expression)) {}

cpplox::AST::PrintStmt::PrintStmt(cpplox::AST::pExpr expression) : expression(std::move(expression)) {}
