#include "Parser.h"
#include "Logger.h"

auto cpplox::Parser::parse() -> std::shared_ptr<Expr> {
    try { return expression(); } catch (ParseError &err) { return nullptr; }
}

auto cpplox::Parser::expression() -> std::shared_ptr<Expr> { return equality(); }

// equality -> comparison ( ( "!=" | "==" ) comparison )*
auto cpplox::Parser::equality() -> std::shared_ptr<Expr> {
    std::shared_ptr<Expr> expr = comparison();
    while (match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)) {
        Token op = previous();
        std::shared_ptr<Expr> right = comparison();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

// comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )*
auto cpplox::Parser::comparison() -> std::shared_ptr<Expr> {
    std::shared_ptr<Expr> expr = term();
    while (match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL)) {
        Token op = previous();
        std::shared_ptr<Expr> right = term();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

// term -> factor ( ( "-" | "+" ) factor )*
auto cpplox::Parser::term() -> std::shared_ptr<Expr> {
    std::shared_ptr<Expr> expr = factor();
    while (match(TokenType::MINUS, TokenType::PLUS)) {
        Token op = previous();
        std::shared_ptr<Expr> right = factor();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

// factor -> unary ( ( "/" | "*" ) unary )*
auto cpplox::Parser::factor() -> std::shared_ptr<Expr> {
    std::shared_ptr<Expr> expr = unary();
    while (match(TokenType::SLASH, TokenType::STAR)) {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

// unary -> ( "!" | "-" ) unary | primary
auto cpplox::Parser::unary() -> std::shared_ptr<Expr> {
    if (match(TokenType::BANG, TokenType::MINUS)) {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        return std::make_shared<UnaryExpr>(op, right);
    }
    return primary();
}

// primary -> NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")"
auto cpplox::Parser::primary() -> std::shared_ptr<Expr> {
    if (match(TokenType::FALSE_TOKEN)) return std::make_shared<LiteralExpr>(false);
    if (match(TokenType::TRUE_TOKEN)) return std::make_shared<LiteralExpr>(true);
    if (match(TokenType::NIL)) return std::make_shared<LiteralExpr>(nullptr);
    if (match(TokenType::NUMBER, TokenType::STRING)) {
        return std::make_shared<LiteralExpr>(previous().literal);
    }
    if (match(TokenType::LEFT_PAREN)) {
        std::shared_ptr<Expr> expr = expression();
        consumeOrError(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_shared<GroupingExpr>(expr);
    }
    // does not match any terminals
    throw error(peek(), "Expect expression.");
}

cpplox::Token cpplox::Parser::consumeOrError(TokenType type, const std::string &message) {
    if (check(type)) return advance();
    throw error(peek(), message);
}

auto cpplox::Parser::error(const Token &token, const std::string &msg) const -> ParseError {
    std::string error = msg;
    if (token.type == TokenType::EOF_TOKEN) error = " at end: " + error;
    else error = " at '" + token.lexeme + "': " + error;
    logger::trace(token.line, "filename", msg);

    return ParseError{""};
}

void cpplox::Parser::synchronize() {
    advance();
    // It discard tokens until it thinks it has found a statement boundary.
    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON) return;

        switch (peek().type) {
            case TokenType::CLASS:
            case TokenType::FUN:
            case TokenType::VAR:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::PRINT:
            case TokenType::RETURN:
                return;
        }

        advance();
    }
}

//  This checks to see if the current token has any of the given types.
//  If so, it consumes the token and returns true.
//  Otherwise, it returns false and leaves the current token alone. 
template<class... T>
bool cpplox::Parser::match(T ... types) {
    if ((... || check(types))) {
        advance();
        return true;
    }
    return false;
}

bool cpplox::Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

cpplox::Token cpplox::Parser::advance() {
    if (!isAtEnd()) { ++current; }
    return previous();
}

bool cpplox::Parser::isAtEnd() { return peek().type == TokenType::EOF_TOKEN; }

cpplox::Token cpplox::Parser::peek() { return tokens[current]; }

cpplox::Token cpplox::Parser::previous() { return tokens[current - 1]; }
