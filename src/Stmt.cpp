#include "Stmt.h"

#include <utility>

cpplox::AST::BlockStmt::BlockStmt(std::vector<pStmt> &&statements)
    : statements(std::move(statements)) {}

cpplox::AST::ExprStmt::ExprStmt(pExpr expression)
    : expression(std::move(expression)) {}

cpplox::AST::FuncStmt::FuncStmt(Token name, std::vector<Token> params, std::vector<pStmt> body)
    : name(std::move(name)), params(std::move(params)), body(std::move(body)) {}

cpplox::AST::IfStmt::IfStmt(pExpr condition, pStmt thenBranch, pStmt elseBranch)
    : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}

cpplox::AST::PrintStmt::PrintStmt(pExpr expression)
    : expression(std::move(expression)) {}

cpplox::AST::VarStmt::VarStmt(Token name, pExpr initializer)
    : name(std::move(name)), initializer(std::move(initializer)) {}

cpplox::AST::WhileStmt::WhileStmt(pExpr condition, pStmt body)
    : condition(std::move(condition)), body(std::move(body)) {}
