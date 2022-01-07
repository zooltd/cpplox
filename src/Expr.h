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

    using pBinaryExpr = std::unique_ptr<BinaryExpr>;
    using pGroupingExpr = std::unique_ptr<GroupingExpr>;
    using pLiteralExpr = std::unique_ptr<LiteralExpr>;
    using pUnaryExpr = std::unique_ptr<UnaryExpr>;

    using pExpr =
            std::variant<pBinaryExpr, pGroupingExpr, pLiteralExpr, pUnaryExpr>;

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

}// namespace cpplox::AST

#endif// CPPLOX_EXPR_H
