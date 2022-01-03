#ifndef CPPLOX_PARSER_H
#define CPPLOX_PARSER_H

#include "Expr.h"
#include "Token.h"
#include <memory>
#include <vector>
#include <string>

namespace cpplox {

    struct ParseError : std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    class Parser {
    public:
        Parser(std::vector<Token> tokens)
            : tokens(std::move(tokens)) {}

        auto parse() -> std::shared_ptr<Expr>;

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

        auto expression() -> std::shared_ptr<Expr>;
        auto equality() -> std::shared_ptr<Expr>;
        auto comparison() -> std::shared_ptr<Expr>;
        auto term() -> std::shared_ptr<Expr>;
        auto factor() -> std::shared_ptr<Expr>;
        auto unary() -> std::shared_ptr<Expr>;
        auto primary() -> std::shared_ptr<Expr>;

        auto error(const Token &token, const std::string &msg) const -> ParseError;
    };


}// namespace cpplox

#endif//CPPLOX_PARSER_H
