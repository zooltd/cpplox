#include "Parser.h"
#include "Logger.h"
#include "Meta.h"

auto cpplox::Parser::parse() -> std::vector<AST::pStmt> {
    std::vector<AST::pStmt> statements;
    try {
        while (!isAtEnd())
            statements.push_back(statement());
    } catch (ParseErr &err) {
        Errors::hadError = true;
        logger::error(err);
    }
    return statements;
}

auto cpplox::Parser::statement() -> cpplox::AST::pStmt {
    if (match(TokenType::PRINT))
        return printStatement();
    return expressionStatement();
}

auto cpplox::Parser::printStatement() -> cpplox::AST::pStmt {
    AST::pExpr value = expression();
    consumeOrError(TokenType::SEMICOLON, "Expect ';' after value.");
    return std::make_unique<AST::PrintStmt>(std::move(value));
}

auto cpplox::Parser::expressionStatement() -> cpplox::AST::pStmt {
    AST::pExpr expr = expression();
    consumeOrError(TokenType::SEMICOLON, "Expect ';' after expression.");
    return std::make_unique<AST::ExpressionStmt>(std::move(expr));
}

// expression -> equality
auto cpplox::Parser::expression() -> AST::pExpr { return equality(); }

// equality -> comparison ( ( "!=" | "==" ) comparison )*
auto cpplox::Parser::equality() -> AST::pExpr {
    AST::pExpr expr = comparison();
    while (match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)) {
        Token op = previous();
        AST::pExpr right = comparison();
        expr = std::make_unique<AST::BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

// comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )*
auto cpplox::Parser::comparison() -> AST::pExpr {
    AST::pExpr expr = term();
    while (match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS,
                 TokenType::LESS_EQUAL)) {
        Token op = previous();
        AST::pExpr right = term();
        expr = std::make_unique<AST::BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

// term -> factor ( ( "-" | "+" ) factor )*
auto cpplox::Parser::term() -> AST::pExpr {
    AST::pExpr expr = factor();
    while (match(TokenType::MINUS, TokenType::PLUS)) {
        Token op = previous();
        AST::pExpr right = factor();
        expr = std::make_unique<AST::BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

// factor -> unary ( ( "/" | "*" ) unary )*
auto cpplox::Parser::factor() -> AST::pExpr {
    AST::pExpr expr = unary();
    while (match(TokenType::SLASH, TokenType::STAR)) {
        Token op = previous();
        AST::pExpr right = unary();
        expr = std::make_unique<AST::BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

// unary -> ( "!" | "-" ) unary | primary
auto cpplox::Parser::unary() -> AST::pExpr {
    if (match(TokenType::BANG, TokenType::MINUS)) {
        Token op = previous();
        AST::pExpr right = unary();
        return std::make_unique<AST::UnaryExpr>(op, std::move(right));
    }
    return primary();
}

// primary -> NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")"
auto cpplox::Parser::primary() -> AST::pExpr {
    if (match(TokenType::FALSE_TOKEN))
        return std::make_unique<AST::LiteralExpr>(false);
    if (match(TokenType::TRUE_TOKEN))
        return std::make_unique<AST::LiteralExpr>(true);
    if (match(TokenType::NIL))
        return std::make_unique<AST::LiteralExpr>(nullptr);
    if (match(TokenType::NUMBER, TokenType::STRING)) {
        return std::make_unique<AST::LiteralExpr>(previous().literal);
    }
    if (match(TokenType::LEFT_PAREN)) {
        AST::pExpr expr = expression();
        consumeOrError(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<AST::GroupingExpr>(std::move(expr));
    }
    // does not match any terminals
    throw error(peek(), "Expect expression.");
}

cpplox::Token cpplox::Parser::consumeOrError(TokenType type,
                                             const std::string &message) {
    if (check(type))
        return advance();
    throw error(peek(), message);
}

auto cpplox::Parser::error(const Token &token, const std::string &msg) const
        -> ParseErr {
    std::string error = msg;
    if (token.type == TokenType::EOF_TOKEN)
        error = " at end: " + error;
    else
        error = " at '" + token.lexeme + "': " + error;
    return ParseErr{Meta::sourceFile, token.line, error};
}

void cpplox::Parser::synchronize() {
    advance();
    // It discard tokens until it thinks it has found a statement boundary.
    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON)
            return;
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
bool cpplox::Parser::match(T... types) {
    if ((check(types) || ...)) {
        advance();
        return true;
    }
    return false;
}

bool cpplox::Parser::check(TokenType type) {
    if (isAtEnd())
        return false;
    return peek().type == type;
}

cpplox::Token cpplox::Parser::advance() {
    if (!isAtEnd()) {
        ++current;
    }
    return previous();
}

bool cpplox::Parser::isAtEnd() { return peek().type == TokenType::EOF_TOKEN; }

cpplox::Token cpplox::Parser::peek() { return tokens[current]; }

cpplox::Token cpplox::Parser::previous() { return tokens[current - 1]; }
