#ifndef CPPLOX_ERRORS_H
#define CPPLOX_ERRORS_H

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace Errors {

    static bool hadError = false;
    static bool hadRuntimeError = false;

    class RuntimeError final : public std::runtime_error {
    public:
        const int line;
        const std::string source;

        RuntimeError(std::string source, const int line, const std::string &msg)
            : std::runtime_error(msg), line(line), source(std::move(source)) {}

        friend std::ostream &operator<<(std::ostream &os, const RuntimeError &err) {
            os << err.source << "(" << err.line << ") : " << err.what();
            return os;
        }
    };

}// namespace Errors
#endif// CPPLOX_ERRORS_H
