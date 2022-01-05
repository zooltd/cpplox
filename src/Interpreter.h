#ifndef CPPLOX_INTERPRETER_H
#define CPPLOX_INTERPRETER_H

#include "Errors.h"
#include "Expr.h"
#include "Logger.h"
#include "Object.h"
#include <memory>
#include <string>

namespace cpplox {
    using InterpretErr = Errors::RuntimeError;

    class Interpreter {
    public:
        void interpret(const pExpr &pExpr);

    private:
        Object evaluate(const pExpr &pExpr);

        Object evalBinaryExpr(const pBinaryExpr &pExpr);
        Object evalGroupingExpr(const pGroupingExpr &pExpr);
        Object evalLiteralExpr(const pLiteralExpr &pExpr);
        Object evalUnaryExpr(const pUnaryExpr &pExpr);

        bool isTruthy(const Object &obj) const;
        void checkNumberOperand(const Token &op, const Object &operand);
        void checkNumberOperands(const Token &op, const Object &left,
                                 const Object &right);
    };

}// namespace cpplox

#endif// CPPLOX_INTERPRETER_H
