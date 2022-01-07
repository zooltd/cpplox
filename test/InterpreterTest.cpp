#include "gtest/gtest.h"

#include "Interpreter.h"
#include "Parser.h"
#include <memory>

using namespace cpplox;

TEST(InterpreterTest, BasicAssertions) {
    Token plus(TokenType::PLUS, "+", std::monostate{}, 0);
    AST::pExpr left = std::make_unique<AST::LiteralExpr>((double) 1);
    AST::pExpr right = std::make_unique<AST::LiteralExpr>((double) 2);
    Interpreter interpreter;
    auto val = interpreter.evaluate(std::make_unique<AST::BinaryExpr>(std::move(left), plus, std::move(right)));
    EXPECT_EQ(std::get<double>(val), (double) 3);
}
