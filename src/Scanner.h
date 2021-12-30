#ifndef CPPLOX_SCANNER_H
#define CPPLOX_SCANNER_H

#include <string>
#include <vector>
#include "Token.h"

namespace cpplox {

    class Scanner {
    public:
        Scanner(std::string source);
        auto scanTokens() -> std::vector<Token>;
    private:
        std::string source;
        std::vector<Token> tokens;
    };

}// namespace cpplox

#endif//CPPLOX_SCANNER_H
