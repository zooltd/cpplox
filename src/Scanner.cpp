#include "Scanner.h"
#include "Logger.h"
#include "Meta.h"

cpplox::Scanner::Scanner(std::string source) : source(std::move(source)) {}

auto cpplox::Scanner::scanTokens() -> std::vector<Token> {
    while (!isAtEnd()) {
        // We are at the beginning of the next lexeme.
        start = current;
        try {
            scanToken();
        } catch (const TokenizationErr &err) {
            Errors::hadError = true;
            logger::error(err);
        }
    }
    tokens.emplace_back(TokenType::EOF_TOKEN, "", std::monostate{}, line);
    return tokens;
}

bool cpplox::Scanner::isAtEnd() const {
    return current >= static_cast<int>(source.length());
}

void cpplox::Scanner::scanToken() {
    switch (const char c = advance()) {
        case '(':
            addToken(TokenType::LEFT_PAREN);
            break;
        case ')':
            addToken(TokenType::RIGHT_PAREN);
            break;
        case '{':
            addToken(TokenType::LEFT_BRACE);
            break;
        case '}':
            addToken(TokenType::RIGHT_BRACE);
            break;
        case ',':
            addToken(TokenType::COMMA);
            break;
        case '.':
            addToken(TokenType::DOT);
            break;
        case '-':
            addToken(TokenType::MINUS);
            break;
        case '+':
            addToken(TokenType::PLUS);
            break;
        case ';':
            addToken(TokenType::SEMICOLON);
            break;
        case '*':
            addToken(TokenType::STAR);
            break;

        case '!':
            addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=':
            addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '<':
            addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '>':
            addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;

        case '/':
            // A comment goes until the end of the line.
            if (match('/'))
                while (peek() != '\n' && !isAtEnd())
                    advance();
            else
                addToken(TokenType::SLASH);
            break;

        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace.
            break;
        case '\n':
            line++;
            break;

        // parse strings
        case '"': {
            if (auto value = parseStr())
                addToken(TokenType::STRING, *value);
            break;
        }

        default: {
            // parse digits
            if (isDigit(c)) {
                if (auto value = parseNum())
                    addToken(TokenType::NUMBER, *value);
            }
            // parse keywords or identifiers
            else if (isAlpha(c)) {
                if (const auto type = parseKeywords())
                    addToken(*type);
                else
                    addToken(TokenType::IDENTIFIER);
            }
            // unknown lexemes
            else
                throw TokenizationErr(Meta::sourceFile, line, "Unexpected character.");
        }
    }
}

bool cpplox::Scanner::match(char expected) {
    if (isAtEnd())
        return false;
    if (source[current] != expected)
        return false;

    current++;
    return true;
}

bool cpplox::Scanner::isDigit(char c) { return c >= '0' && c <= '9'; }

bool cpplox::Scanner::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool cpplox::Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

char cpplox::Scanner::peek() const {
    if (isAtEnd())
        return '\0';
    return source[current];
}

char cpplox::Scanner::peekNext() const {
    if (current + 1 >= static_cast<int>(source.length()))
        return '\0';
    return source[current + 1];
}

auto cpplox::Scanner::parseStr() -> std::optional<std::string> {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n')
            line++;
        advance();
    }
    if (isAtEnd()) {
        logger::trace(line, source, "Unterminated string.");
        return std::nullopt;
    }

    // The closing ".
    advance();

    return source.substr(start + 1, current - start - 2);
}

auto cpplox::Scanner::parseNum() -> std::optional<double> {
    while (isDigit(peek()))
        advance();
    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
        // Consume the "."
        advance();
        while (isDigit(peek()))
            advance();
    }
    return std::stod(source.substr(start, current - start));
}

auto cpplox::Scanner::parseKeywords() -> std::optional<TokenType> {
    while (isAlphaNumeric(peek()))
        advance();
    const std::string text = source.substr(start, current - start);
    auto type = keywords.find(text);
    if (type != keywords.end())
        return (*type).second;
    return std::nullopt;
}

char cpplox::Scanner::advance() { return source[current++]; }

void cpplox::Scanner::addToken(TokenType type) {
    addToken(type, std::monostate{});
}

void cpplox::Scanner::addToken(TokenType type, const Object &literal) {
    std::string text = source.substr(start, current - start);
    tokens.emplace_back(type, text, literal, line);
}
