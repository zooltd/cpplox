#ifndef CPPLOX_EXPR_H
#define CPPLOX_EXPR_H

#include "Object.h"
#include "Token.h"
#include <memory>
#include <variant>

namespace cpplox::AST {

    class BinaryExpr;
    class GroupingExpr;
    class LiteralExpr;
    class UnaryExpr;
    class VariableExpr;

    using pBinaryExpr = std::unique_ptr<BinaryExpr>;
    using pGroupingExpr = std::unique_ptr<GroupingExpr>;
    using pLiteralExpr = std::unique_ptr<LiteralExpr>;
    using pUnaryExpr = std::unique_ptr<UnaryExpr>;
    using pVariableExpr = std::unique_ptr<VariableExpr>;

    using pExpr = std::variant<std::nullptr_t, pBinaryExpr, pGroupingExpr, pLiteralExpr, pUnaryExpr, pVariableExpr>;

    class BinaryExpr {
    public:
        const pExpr left;
        const Token op;
        const pExpr right;
        BinaryExpr(pExpr left, Token op, pExpr right);
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
