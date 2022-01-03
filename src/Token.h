#ifndef CPPLOX_TOKEN_H
#define CPPLOX_TOKEN_H

#include <utility>
#include <variant>
#include <string>
#include <ostream>
#include "magic_enum.hpp"

namespace cpplox {

    enum class TokenType {
        // Single-character tokens.
        LEFT_PAREN,
        RIGHT_PAREN,
        LEFT_BRACE,
        RIGHT_BRACE,
        COMMA,
        DOT,
        MINUS,
        PLUS,
        SEMICOLON,
        SLASH,
        STAR,

        // One or two character tokens.
        BANG,
        BANG_EQUAL,
        EQUAL,
        EQUAL_EQUAL,
        GREATER,
        GREATER_EQUAL,
        LESS,
        LESS_EQUAL,

        // Literals.
        IDENTIFIER,
        STRING,
        NUMBER,

        // Keywords.
        AND,
        CLASS,
        ELSE,
        FALSE_TOKEN,
        FUN,
        FOR,
        IF,
        NIL,
        OR,
        PRINT,
        RETURN,
        SUPER,
        THIS,
        TRUE_TOKEN,
        VAR,
        WHILE,

        EOF_TOKEN
    };

    // helper type for the visitor #4
    template<class... Ts>
    struct overloaded : Ts... {
        using Ts::operator()...;
    };

    // explicit deduction guide (not needed as of C++20)
    template<class... Ts>
    overloaded(Ts ...) -> overloaded<Ts...>;

    template<typename T, typename... Ts>
    std::ostream &operator<<(std::ostream &os, const std::variant<T, Ts...> &v) {
        std::visit(
                overloaded{
                        [&os](std::monostate) { os << "Monostate"; },
                        [&os](auto &&arg) { os << arg; }},
                v);
        return os;
    }

    using Literal = std::variant<std::monostate, double, std::string>;

    class Token {
    public:
        Token(TokenType type, std::string lexeme, Literal literal, int line)
            : type(type), lexeme(std::move(lexeme)), literal(std::move(literal)), line(line) {}

        friend std::ostream &operator<<(std::ostream &os, const Token &token) {
            os << magic_enum::enum_name(token.type) << " " << token.lexeme << " " << token.literal;
            return os;
        }

        const TokenType type;
        const std::string lexeme;
        const Literal literal;
        const int line;
    };
}// namespace cpplox

#endif//CPPLOX_TOKEN_H
