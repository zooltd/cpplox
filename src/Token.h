#ifndef CPPLOX_TOKEN_H
#define CPPLOX_TOKEN_H

#include <utility>
#include <variant>
#include <string>
#include <ostream>
#include "magic_enum.hpp"

class Token {
public:
    Token() {}

    friend std::ostream &operator<<(std::ostream &os, const Token &token) { return os; }

};

#endif//CPPLOX_TOKEN_H
