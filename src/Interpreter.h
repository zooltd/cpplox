#ifndef CPPLOX_INTERPRETER_H
#define CPPLOX_INTERPRETER_H

#include "Errors.h"
#include "Expr.h"
#include "Logger.h"
#include "Object.h"
#include "Stmt.h"
#include "Environment.h"
#include <memory>
#include <string>
#include <vector>

namespace cpplox {
    using InterpretErr = Errors::Err;

    class Interpreter {
    public:
        Interpreter();
        void interpret(const std::vector<AST::pStmt> &statements);
        Object evaluate(const AST::pExpr &pExpr);
        void execute(const AST::pStmt &pStmt);

    private:
        pEnv environment;

        void evalBlockStmt(const AST::pBlockStmt &pStmt, pEnv blockEnv);
        void evalExpressionStmt(const AST::pExpressionStmt &pStmt);
        void evalIfStmt(const AST::pIfStmt &pStmt);
        void evalPrintStmt(const AST::pPrintStmt &pStmt);
        void evalVarStmt(const AST::pVarStmt &pStmt);

        Object evalAssignExpr(const AST::pAssignExpr &pExpr);
        Object evalBinaryExpr(const AST::pBinaryExpr &pExpr);
        Object evalGroupingExpr(const AST::pGroupingExpr &pExpr);
        Object evalLiteralExpr(const AST::pLiteralExpr &pExpr);
        Object evalUnaryExpr(const AST::pUnaryExpr &pExpr);
        Object evalVariableExpr(const AST::pVariableExpr &pExpr);
        Object evalLogicalExpr(const AST::pLogicalExpr &pExpr);

        bool isTruthy(const Object &obj) const;
        void checkNumberOperand(const Token &op, const Object &operand);
        void checkNumberOperands(const Token &op, const Object &left, const Object &right);
    };

}// namespace cpplox

#endif// CPPLOX_INTERPRETER_H
