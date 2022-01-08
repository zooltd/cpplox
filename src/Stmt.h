#ifndef CPPLOX_STMT_H
#define CPPLOX_STMT_H

#include "Expr.h"
#include <memory>
#include <variant>

namespace cpplox::AST {

    class ExpressionStmt;
    class PrintStmt;
    class VarStmt;

    using pExpressionStmt = std::unique_ptr<ExpressionStmt>;
    using pPrintStmt = std::unique_ptr<PrintStmt>;
    using pVarStmt = std::unique_ptr<VarStmt>;

    using pStmt = std::variant<std::nullptr_t, pExpressionStmt, pPrintStmt, pVarStmt>;

    class ExpressionStmt {
    public:
        const pExpr expression;
        explicit ExpressionStmt(pExpr expression);
    };

    class PrintStmt {
    public:
        const pExpr expression;
        explicit PrintStmt(pExpr expression);
    };

    class VarStmt {
    public:
        const Token name;
        const pExpr initializer;
        VarStmt(Token name, pExpr initializer);
    };

}// namespace cpplox::AST

#endif//CPPLOX_STMT_H
