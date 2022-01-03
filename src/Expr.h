#ifndef CPPLOX_EXPR_H
#define CPPLOX_EXPR_H

#include "Token.h"
#include <vector>
#include <any>
#include <memory>

namespace cpplox {

    class AssignExpr;
    class BinaryExpr;
    class CallExpr;
    class GetExpr;
    class GroupingExpr;
    class LiteralExpr;
    class LogicalExpr;
    class SetExpr;
    class SuperExpr;
    class ThisExpr;
    class UnaryExpr;
    class VariableExpr;

    class Visitor {
    public:
        virtual std::any visitAssignExpr(std::shared_ptr<AssignExpr> expr) = 0;
        virtual std::any visitBinaryExpr(std::shared_ptr<BinaryExpr> expr) = 0;
        virtual std::any visitCallExpr(std::shared_ptr<CallExpr> expr) = 0;
        virtual std::any visitGetExpr(std::shared_ptr<GetExpr> expr) = 0;
        virtual std::any visitGroupingExpr(std::shared_ptr<GroupingExpr> expr) = 0;
        virtual std::any visitLiteralExpr(std::shared_ptr<LiteralExpr> expr) = 0;
        virtual std::any visitLogicalExpr(std::shared_ptr<LogicalExpr> expr) = 0;
        virtual std::any visitSetExpr(std::shared_ptr<SetExpr> expr) = 0;
        virtual std::any visitSuperExpr(std::shared_ptr<SuperExpr> expr) = 0;
        virtual std::any visitThisExpr(std::shared_ptr<ThisExpr> expr) = 0;
        virtual std::any visitUnaryExpr(std::shared_ptr<UnaryExpr> expr) = 0;
        virtual std::any visitVariableExpr(std::shared_ptr<VariableExpr> expr) = 0;
        virtual ~Visitor() = default;
    };

    class Expr {
    public:
        virtual ~Expr() = default;
        virtual std::any accept(Visitor &visitor) = 0;
    };

    class AssignExpr : public Expr, public std::enable_shared_from_this<AssignExpr> {
    public:
        AssignExpr(Token name, std::shared_ptr<Expr> value)
            : name(std::move(name)), value(std::move(value)) { }

        std::any accept(Visitor &visitor) override {
            return visitor.visitAssignExpr(shared_from_this());
        }

    private:
        const Token name;
        const std::shared_ptr<Expr> value;
    };

    class BinaryExpr : public Expr, public std::enable_shared_from_this<BinaryExpr> {
    public:
        BinaryExpr(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
            : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

        std::any accept(Visitor &visitor) override {
            return visitor.visitBinaryExpr(shared_from_this());
        }

    private:
        const std::shared_ptr<Expr> left;
        const Token op;
        const std::shared_ptr<Expr> right;
    };

    class UnaryExpr : public Expr, public std::enable_shared_from_this<UnaryExpr> {
    public:
        UnaryExpr(Token op, std::shared_ptr<Expr> right)
            : op(std::move(op)), right(std::move(right)) {}

        std::any accept(Visitor &visitor) override { return visitor.visitUnaryExpr(shared_from_this()); }

    private:
        const Token op;
        const std::shared_ptr<Expr> right;
    };

    class LiteralExpr : public Expr, public std::enable_shared_from_this<LiteralExpr> {
    public:
        LiteralExpr(std::any value)
            : value(std::move(value)) {}

        std::any accept(Visitor &visitor) override {
            return visitor.visitLiteralExpr(shared_from_this());
        }

    private:
        const std::any value;
    };

    class GroupingExpr : public Expr, public std::enable_shared_from_this<GroupingExpr> {
    public:
        GroupingExpr(std::shared_ptr<Expr> expression)
            : expression(std::move(expression)) {}

        std::any accept(Visitor &visitor) override {
            return visitor.visitGroupingExpr(shared_from_this());
        }

    private:
        const std::shared_ptr<Expr> expression;
    };

    class CallExpr : public Expr, std::enable_shared_from_this<CallExpr> {
    public:
        CallExpr(std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments)
            : callee(std::move(callee)), paren(std::move(paren)), arguments(std::move(arguments)) {}

        std::any accept(Visitor &visitor) override { return visitor.visitCallExpr(shared_from_this()); }

    private:
        const std::shared_ptr<Expr> callee;
        const Token paren;
        const std::vector<std::shared_ptr<Expr>> arguments;
    };


}// namespace Expr

#endif//CPPLOX_EXPR_H
