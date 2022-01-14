#ifndef CPPLOX_STMT_H
#define CPPLOX_STMT_H

#include "Expr.h"
#include <memory>
#include <variant>
#include <vector>

namespace cpplox::AST {

    class BlockStmt;
    class ExprStmt;
    class FuncStmt;
    class IfStmt;
    class PrintStmt;
    class VarStmt;
    class WhileStmt;

    using pBlockStmt = std::unique_ptr<BlockStmt>;
    using pExpressionStmt = std::unique_ptr<ExprStmt>;
    using pFunctionStmt = std::unique_ptr<FuncStmt>;
    using pIfStmt = std::unique_ptr<IfStmt>;
    using pPrintStmt = std::unique_ptr<PrintStmt>;
    using pVarStmt = std::unique_ptr<VarStmt>;
    using pWhileStmt = std::unique_ptr<WhileStmt>;

    using pStmt = std::variant<std::nullptr_t, pBlockStmt, pExpressionStmt, pFunctionStmt, pIfStmt, pPrintStmt, pVarStmt, pWhileStmt>;

    class BlockStmt {
    public:
        const std::vector<pStmt> statements;
        explicit BlockStmt(std::vector<pStmt> &&statements);
    };

    class ExprStmt {
    public:
        const pExpr expression;
        explicit ExprStmt(pExpr expression);
    };

    class FuncStmt {
    public:
        const Token name;
        const std::vector<Token> params;
        const std::vector<pStmt> body;
        FuncStmt(Token name, std::vector<Token> params, std::vector<pStmt> body);
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

    class WhileStmt {
    public:
        const pExpr condition;
        const pStmt body;
        WhileStmt(pExpr condition, pStmt body);
    };

}// namespace cpplox::AST

#endif//CPPLOX_STMT_H
