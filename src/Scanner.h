#ifndef CPPLOX_SCANNER_H
#define CPPLOX_SCANNER_H

#include "Errors.h"
#include "Object.h"
#include "Token.h"
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace cpplox {

    using TokenizationErr = Errors::RuntimeError;

    static const std::unordered_map<std::string, TokenType> keywords = {
            {"and", TokenType::AND},
            {"class", TokenType::CLASS},
            {"else", TokenType::ELSE},
            {"false", TokenType::FALSE_TOKEN},
            {"for", TokenType::FOR},
            {"fun", TokenType::FUN},
            {"if", TokenType::IF},
            {"nil", TokenType::NIL},
            {"or", TokenType::OR},
            {"print", TokenType::PRINT},
            {"return", TokenType::RETURN},
            {"super", TokenType::SUPER},
            {"this", TokenType::THIS},
            {"true", TokenType::TRUE_TOKEN},
            {"var", TokenType::VAR},
            {"while", TokenType::WHILE}};

    class Scanner {
    public:
        explicit Scanner(std::string source);
        auto scanTokens() -> std::vector<Token>;

    private:
        std::string source;
        std::vector<Token> tokens;
        // the first character in the lexeme being scanned
        int start = 0;
        // the character currently being considered
        int current = 0;
        // tracks what source line current is on
        int line = 1;

        char advance();
        char peek() const;
        char peekNext() const;
        bool isAtEnd() const;
        bool match(char expected);
        bool isDigit(char c);
        bool isAlpha(char c);
        bool isAlphaNumeric(char c);
        void scanToken();
        void addToken(TokenType type);
        void addToken(TokenType type, const Object &literal);
        auto parseStr() -> std::optional<std::string>;
        auto parseNum() -> std::optional<double>;
        auto parseKeywords() -> std::optional<TokenType>;
    };

}// namespace cpplox

#endif// CPPLOX_SCANNER_H
