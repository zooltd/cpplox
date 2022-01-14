#ifndef CPPLOX_OBJECT_H
#define CPPLOX_OBJECT_H

#include <iostream>
#include <string>
#include <variant>
#include <memory>
#include <vector>

namespace cpplox {
    class Interpreter;
    class Function;
    class Callable;

    using pCallable = std::shared_ptr<Callable>;
    using pFunction = std::shared_ptr<Function>;

    using Object = std::variant<std::monostate, std::string, double, bool, pCallable>;

    class Callable {
    public:
        virtual int arity() = 0;
        virtual Object call(Interpreter &interpreter, const std::vector<Object> &arguments) = 0;
        virtual std::string toString() = 0;
    };

    template<class... Ts>
    struct overloaded : Ts... {
        using Ts::operator()...;
    };

    template<class... Ts>
    overloaded(Ts ...) -> overloaded<Ts...>;

    template<typename T, typename... Ts>
    std::ostream &operator<<(std::ostream &os, const std::variant<T, Ts...> &v) {
        std::visit(
                overloaded{
                        [&os](std::monostate) { os << "NULL"; },
                        [&os](const pCallable &p) { os << p->toString(); },
                        [&os](bool arg) { os << (arg ? "TRUE" : "FALSE"); },
                        [&os](auto &&arg) { os << arg; }},
                v);
        return os;
    }
}// namespace cpplox

#endif// CPPLOX_OBJECT_H
