#ifndef CPPLOX_STMT_H
#define CPPLOX_STMT_H

#include "Expr.h"
#include <memory>
#include <variant>

namespace cpplox::AST {

    class ExpressionStmt;
    class PrintStmt;

    using pExpressionStmt = std::unique_ptr<ExpressionStmt>;
    using pPrintStmt = std::unique_ptr<PrintStmt>;

    using pStmt = std::variant<pExpressionStmt, pPrintStmt>;

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

}// namespace cpplox::AST

#endif//CPPLOX_STMT_H
