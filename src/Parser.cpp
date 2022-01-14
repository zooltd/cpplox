#include "Parser.h"
#include "Logger.h"
#include "Meta.h"

// program -> declaration* EOF
auto cpplox::Parser::parse() -> std::vector<AST::pStmt> {
    std::vector<AST::pStmt> statements;
    while (!isAtEnd()) statements.push_back(declaration());
    return statements;
}

// declaration -> funDecl | varDecl | statement
auto cpplox::Parser::declaration() -> AST::pStmt {
    try {
        if (match(TokenType::FUN)) return function("function");
        if (match(TokenType::VAR)) return varDeclaration();
        return statement();
    } catch (const ParseErr &err) {
        logger::error(err);
        synchronize();
        return nullptr;
    }
}

// funDecl -> "fun" function
// function -> IDENTIFIER "(" parameters? ")" block
// parameters -> IDENTIFIER ( "," IDENTIFIER )*
auto cpplox::Parser::function(const std::string &kind) -> AST::pStmt {
    Token name = consumeOrError(TokenType::IDENTIFIER, "Expect " + kind + " name.");

    consumeOrError(TokenType::LEFT_PAREN, "Expect '(' after " + kind + " name.");
    std::vector<Token> parameters;
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (parameters.size() >= 255) error(peek(), "Can't have more than 255 parameters.");
            parameters.emplace_back(consumeOrError(TokenType::IDENTIFIER, "Expect parameter name."));
        } while (match(TokenType::COMMA));
    }
    consumeOrError(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");
    consumeOrError(TokenType::LEFT_BRACE, "Expect '{' before " + kind + " body.");
    std::vector<AST::pStmt> body = block();
    return std::make_unique<AST::FuncStmt>(std::move(name), std::move(parameters), std::move(body));
}

// varDecl -> "var" IDENTIFIER ( "=" expression )? ";"
auto cpplox::Parser::varDeclaration() -> AST::pStmt {
    Token name = consumeOrError(TokenType::IDENTIFIER, "Expect variable name.");
    AST::pExpr initializer = nullptr;
    if (match(TokenType::EQUAL)) initializer = expression();
    consumeOrError(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_unique<AST::VarStmt>(std::move(name), std::move(initializer));
}

// statement -> exprStmt | forStmt | ifStmt | printStmt | whileStmt | block
auto cpplox::Parser::statement() -> AST::pStmt {
    if (match(TokenType::FOR)) return forStatement();
    if (match(TokenType::IF)) return ifStatement();
    if (match(TokenType::PRINT)) return printStatement();
    if (match(TokenType::WHILE)) return whileStatement();
    if (match(TokenType::LEFT_BRACE)) return blockStatement();
    return expressionStatement();
}

// forStmt -> "for" "(" ( varDecl | exprStmt | ";" ) expression? ";" expression? ")" statement
auto cpplox::Parser::forStatement() -> AST::pStmt {
    consumeOrError(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");

    AST::pStmt initializer;
    if (match(TokenType::SEMICOLON)) initializer = nullptr;
    else if (match(TokenType::VAR)) initializer = varDeclaration();
    else initializer = expressionStatement();

    AST::pExpr condition = nullptr;
    if (!check(TokenType::SEMICOLON)) condition = expression();

    consumeOrError(TokenType::SEMICOLON, "Expect ';' after loop condition.");

    AST::pExpr increment = nullptr;
    if (!check(TokenType::RIGHT_PAREN)) increment = expression();

    consumeOrError(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");

    AST::pStmt body = statement();

    if (!std::holds_alternative<std::nullptr_t>(increment)) {
        std::vector<AST::pStmt> stmts;
        stmts.push_back(std::move(body));
        stmts.emplace_back(std::make_unique<AST::ExprStmt>(std::move(increment)));
        body = std::make_unique<AST::BlockStmt>(std::move(stmts));
    }

    if (std::holds_alternative<std::nullptr_t>(condition)) condition = std::make_unique<AST::LiteralExpr>(true);
    body = std::make_unique<AST::WhileStmt>(std::move(condition), std::move(body));

    if (!std::holds_alternative<std::nullptr_t>(initializer)) {
        std::vector<AST::pStmt> stmts;
        stmts.push_back(std::move(initializer));
        stmts.push_back(std::move(body));
        body = std::make_unique<AST::BlockStmt>(std::move(stmts));
    }

    return body;
}

// ifStmt -> "if" "(" expression ")" statement ( "else" statement )?
// the else is bound to the nearest if that precedes it
// to handle the dangling else problem
auto cpplox::Parser::ifStatement() -> AST::pStmt {
    consumeOrError(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
    AST::pExpr condition = expression();
    consumeOrError(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");

    AST::pStmt thenBranch = statement();
    AST::pStmt elseBranch = nullptr;
    if (match(TokenType::ELSE)) elseBranch = statement();
    return std::make_unique<AST::IfStmt>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

// printStmt -> "print" expression ";"
auto cpplox::Parser::printStatement() -> AST::pStmt {
    AST::pExpr value = expression();
    consumeOrError(TokenType::SEMICOLON, "Expect ';' after value.");
    return std::make_unique<AST::PrintStmt>(std::move(value));
}

// whileStmt -> "while" "(" expression ")" statement
auto cpplox::Parser::whileStatement() -> AST::pStmt {
    consumeOrError(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
    AST::pExpr condition = expression();
    consumeOrError(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
    AST::pStmt body = statement();

    return std::make_unique<AST::WhileStmt>(std::move(condition), std::move(body));
}

// block -> "{" declaration* "}"
auto cpplox::Parser::blockStatement() -> AST::pStmt { return std::make_unique<AST::BlockStmt>(block()); }

auto cpplox::Parser::block() -> std::vector<AST::pStmt> {
    std::vector<AST::pStmt> statements;
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) { statements.emplace_back(declaration()); }
    consumeOrError(TokenType::RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

// exprStmt -> expression ";"
auto cpplox::Parser::expressionStatement() -> AST::pStmt {
    AST::pExpr expr = expression();
    consumeOrError(TokenType::SEMICOLON, "Expect ';' after expression.");
    return std::make_unique<AST::ExprStmt>(std::move(expr));
}

// expression -> assignment
auto cpplox::Parser::expression() -> AST::pExpr { return assignment(); }

// assignment -> IDENTIFIER "=" assignment | logic_or
auto cpplox::Parser::assignment() -> AST::pExpr {
    AST::pExpr expr = logical_or();

    if (match(TokenType::EQUAL)) {
        const Token equals = previous();
        AST::pExpr value = assignment();

        if (std::holds_alternative<AST::pVariableExpr>(expr)) {
            Token name = std::get<AST::pVariableExpr>(expr)->name;
            return std::make_unique<AST::AssignExpr>(std::move(name), std::move(value));
        }

        auto _ = error(equals, "Invalid assignment target.");
    }

    return expr;
}

// logic_or -> logic_and ( "or" logic_and )*
auto cpplox::Parser::logical_or() -> AST::pExpr {
    AST::pExpr expr = logical_and();

    while (match(TokenType::OR)) {
        const Token op = previous();
        AST::pExpr right = logical_and();
        expr = std::make_unique<AST::LogicalExpr>(std::move(expr), op, std::move(right));
    }

    return expr;
}

// logic_and -> equality ( "and" equality )*
auto cpplox::Parser::logical_and() -> AST::pExpr {
    AST::pExpr expr = equality();

    while (match(TokenType::AND)) {
        const Token op = previous();
        AST::pExpr right = equality();
        expr = std::make_unique<AST::LogicalExpr>(std::move(expr), op, std::move(right));
    }

    return expr;
}

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

// unary -> ( "!" | "-" ) unary | call
auto cpplox::Parser::unary() -> AST::pExpr {
    if (match(TokenType::BANG, TokenType::MINUS)) {
        Token op = previous();
        AST::pExpr right = unary();
        return std::make_unique<AST::UnaryExpr>(op, std::move(right));
    }
    return call();
}

// call -> primary ( "(" arguments? ")" )*
// arguments -> expression ( "," expression )*
auto cpplox::Parser::call() -> AST::pExpr {
    AST::pExpr expr = primary();
    while (true) {
        if (!match(TokenType::LEFT_PAREN)) break;

        std::vector<AST::pExpr> arguments;
        if (!check(TokenType::RIGHT_PAREN)) {
            do {
                // maximum argument counts
                if (arguments.size() >= MAX_ARG_LIMIT) { auto _ = error(peek(), "Can't have more than " STRINGIFY(MAX_ARG_LIMIT) " arguments."); }
                arguments.push_back(expression());
            } while (match(TokenType::COMMA));
        }
        Token paren = consumeOrError(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");
        expr = std::make_unique<AST::CallExpr>(std::move(expr), std::move(paren), std::move(arguments));
    }
    return expr;
}

// the argument list


// primary -> NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")" | IDENTIFIER
auto cpplox::Parser::primary() -> AST::pExpr {
    if (match(TokenType::FALSE_TOKEN)) return std::make_unique<AST::LiteralExpr>(false);
    if (match(TokenType::TRUE_TOKEN)) return std::make_unique<AST::LiteralExpr>(true);
    if (match(TokenType::NIL)) return std::make_unique<AST::LiteralExpr>(std::monostate{});
    if (match(TokenType::NUMBER, TokenType::STRING)) return std::make_unique<AST::LiteralExpr>(previous().literal);
    if (match(TokenType::IDENTIFIER)) return std::make_unique<AST::VariableExpr>(previous());
    if (match(TokenType::LEFT_PAREN)) {
        AST::pExpr expr = expression();
        consumeOrError(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<AST::GroupingExpr>(std::move(expr));
    }
    // does not match any terminals
    throw error(peek(), "Expect expression.");
}

cpplox::Token cpplox::Parser::consumeOrError(TokenType type, const std::string &message) {
    if (check(type)) return advance();
    throw error(peek(), message);
}

auto cpplox::Parser::error(const Token &token, const std::string &msg) const -> ParseErr {
    Errors::hadError = true;
    std::string error = msg;
    if (token.type == TokenType::EOF_TOKEN) error = " at end: " + error;
    else error = " at '" + token.lexeme + "': " + error;
    return ParseErr{Meta::sourceFile, token.line, error};
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
template<
    class

    ... T>


bool cpplox::Parser::match(T ... types) {
    if ((check(types) || ...)) {
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
