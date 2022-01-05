#ifndef CPPLOX_PARSER_H
#define CPPLOX_PARSER_H

#include "Errors.h"
#include "Expr.h"
#include "Token.h"
#include <memory>
#include <string>
#include <vector>

namespace cpplox {

    using ParseErr = Errors::RuntimeError;

    class Parser {
    public:
        explicit Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

        auto parse() -> pExpr;

    private:
        std::vector<Token> tokens;
        // the next token waiting to be parsed
        int current = 0;

        template<class... T>
        bool match(T... types);
        bool check(TokenType type);
        bool isAtEnd();
        void synchronize();

        Token peek();
        Token previous();
        Token advance();
        Token consumeOrError(TokenType type, const std::string &message);

        auto expression() -> pExpr;
        auto equality() -> pExpr;
        auto comparison() -> pExpr;
        auto term() -> pExpr;
        auto factor() -> pExpr;
        auto unary() -> pExpr;
        auto primary() -> pExpr;

        auto error(const Token &token, const std::string &msg) const -> ParseErr;
    };

}// namespace cpplox

#endif// CPPLOX_PARSER_H
