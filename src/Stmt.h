#ifndef CPPLOX_STMT_H
#define CPPLOX_STMT_H

#include "Expr.h"
#include <memory>
#include <variant>
#include <vector>

namespace cpplox::AST {

    class BlockStmt;
    class ExpressionStmt;
    class IfStmt;
    class PrintStmt;
    class VarStmt;

    using pBlockStmt = std::unique_ptr<BlockStmt>;
    using pExpressionStmt = std::unique_ptr<ExpressionStmt>;
    using pIfStmt = std::unique_ptr<IfStmt>;
    using pPrintStmt = std::unique_ptr<PrintStmt>;
    using pVarStmt = std::unique_ptr<VarStmt>;

    using pStmt = std::variant<std::nullptr_t, pBlockStmt, pExpressionStmt, pIfStmt, pPrintStmt, pVarStmt>;

    class BlockStmt {
    public:
        const std::vector<pStmt> statements;
        explicit BlockStmt(std::vector<pStmt> statements);
    };

    class ExpressionStmt {
    public:
        const pExpr expression;
        explicit ExpressionStmt(pExpr expression);
    };

    class IfStmt {
    public:
        const pExpr condition;
        const pStmt thenBranch;
        const pStmt elseBranch;
        IfStmt(pExpr condition, pStmt thenBranch, pStmt elseBranch);
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
