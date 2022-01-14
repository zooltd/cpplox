#ifndef CPPLOX_EXPR_H
#define CPPLOX_EXPR_H

#include "Object.h"
#include "Token.h"
#include <memory>
#include <variant>
#include <vector>

namespace cpplox::AST {

    class AssignExpr;
    class BinaryExpr;
    class CallExpr;
    class GroupingExpr;
    class LiteralExpr;
    class LogicalExpr;
    class UnaryExpr;
    class VariableExpr;

    using pAssignExpr = std::unique_ptr<AssignExpr>;
    using pBinaryExpr = std::unique_ptr<BinaryExpr>;
    using pCallExpr = std::unique_ptr<CallExpr>;
    using pGroupingExpr = std::unique_ptr<GroupingExpr>;
    using pLiteralExpr = std::unique_ptr<LiteralExpr>;
    using pLogicalExpr = std::unique_ptr<LogicalExpr>;
    using pUnaryExpr = std::unique_ptr<UnaryExpr>;
    using pVariableExpr = std::unique_ptr<VariableExpr>;

    using pExpr = std::variant<std::nullptr_t, pAssignExpr, pBinaryExpr, pCallExpr, pGroupingExpr, pLiteralExpr, pLogicalExpr, pUnaryExpr, pVariableExpr>;

    class AssignExpr {
    public:
        const Token name;
        const pExpr value;
        AssignExpr(Token name, pExpr value);
    };

    class BinaryExpr {
    public:
        const pExpr left;
        const Token op;
        const pExpr right;
        BinaryExpr(pExpr left, Token op, pExpr right);
    };

    class CallExpr {
    public:
        const pExpr callee;
        const Token paren;
        const std::vector<pExpr> arguments;
        CallExpr(pExpr callee,Token paren , std::vector<pExpr> arguments);
    };

    class UnaryExpr {
    public:
        const Token op;
        const pExpr right;
        UnaryExpr(Token op, pExpr right);
    };

    class LiteralExpr {
    public:
        const Object value;
        explicit LiteralExpr(Object value);
    };

    class LogicalExpr {
    public:
        const pExpr left;
        const Token op;
        const pExpr right;
        LogicalExpr(pExpr left, Token op, pExpr right);
    };

    class GroupingExpr {
    public:
        const pExpr expression;
        explicit GroupingExpr(pExpr expression);
    };

    class VariableExpr {
    public:
        const Token name;
        explicit VariableExpr(Token name);
    };

}// namespace cpplox::AST

#endif// CPPLOX_EXPR_H
