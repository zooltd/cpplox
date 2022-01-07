#ifndef CPPLOX_INTERPRETER_H
#define CPPLOX_INTERPRETER_H

#include "Errors.h"
#include "Expr.h"
#include "Logger.h"
#include "Object.h"
#include "Stmt.h"
#include <memory>
#include <string>
#include <vector>

namespace cpplox {
    using InterpretErr = Errors::RuntimeError;

    class Interpreter {
    public:
        void interpret(const std::vector<AST::pStmt> &statements);
        Object evaluate(const AST::pExpr &pExpr);
        void execute(const AST::pStmt &pStmt);

    private:
        void evalExpressionStmt(const AST::pExpressionStmt &pStmt);
        void evalPrintStmt(const AST::pPrintStmt &pStmt);

        Object evalBinaryExpr(const AST::pBinaryExpr &pExpr);
        Object evalGroupingExpr(const AST::pGroupingExpr &pExpr);
        Object evalLiteralExpr(const AST::pLiteralExpr &pExpr);
        Object evalUnaryExpr(const AST::pUnaryExpr &pExpr);

        bool isTruthy(const Object &obj) const;
        void checkNumberOperand(const Token &op, const Object &operand);
        void checkNumberOperands(const Token &op, const Object &left,
                                 const Object &right);
    };

}// namespace cpplox

#endif// CPPLOX_INTERPRETER_H
