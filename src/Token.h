#ifndef CPPLOX_TOKEN_H
#define CPPLOX_TOKEN_H

#include "lib/magic_enum/include/magic_enum.hpp"
#include "Object.h"
#include <ostream>
#include <string>
#include <utility>
#include <variant>

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

    class Token {
    public:
        Token(TokenType type, std::string lexeme, Object literal, int line)
            : type(type), lexeme(std::move(lexeme)), literal(std::move(literal)),
              line(line) {}

        friend std::ostream &operator<<(std::ostream &os, const Token &token) {
            os << magic_enum::enum_name(token.type) << ", " << token.lexeme << ", "
               << token.literal;
            return os;
        }

        const TokenType type;
        const std::string lexeme;
        const Object literal;
        const int line;
    };
}// namespace cpplox

#endif// CPPLOX_TOKEN_H
