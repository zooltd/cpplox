#ifndef CPPLOX_PARSER_H
#define CPPLOX_PARSER_H

#include "Errors.h"
#include "Expr.h"
#include "Stmt.h"
#include "Token.h"
#include <string>
#include <vector>

namespace cpplox {

    using ParseErr = Errors::Err;

    class Parser {
    public:
        explicit Parser(std::vector<Token> tokens)
            : tokens(std::move(tokens)) {}

        auto parse() -> std::vector<AST::pStmt>;

    private:
        std::vector<Token> tokens;
        // the next token waiting to be parsed
        int current = 0;

        template<class... T>
        bool match(T ... types);
        bool check(TokenType type);
        bool isAtEnd();
        void synchronize();

        Token peek();
        Token previous();
        Token advance();
        Token consumeOrError(TokenType type, const std::string &message);

        auto declaration() -> AST::pStmt;
        auto varDeclaration() -> AST::pStmt;
        auto statement() -> AST::pStmt;
        auto ifStatement() -> AST::pStmt;
        auto printStatement() -> AST::pStmt;
        auto expressionStatement() -> AST::pStmt;
        auto blockStatement() -> AST::pStmt;

        auto assignment() -> AST::pExpr;
        auto expression() -> AST::pExpr;
        auto equality() -> AST::pExpr;
        auto comparison() -> AST::pExpr;
        auto term() -> AST::pExpr;
        auto factor() -> AST::pExpr;
        auto unary() -> AST::pExpr;
        auto primary() -> AST::pExpr;
        auto logical_or() ->AST::pExpr;
        auto logical_and() ->AST::pExpr;

        auto error(const Token &token, const std::string &msg) const -> ParseErr;
    };

}// namespace cpplox

#endif// CPPLOX_PARSER_H
