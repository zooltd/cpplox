#ifndef CPPLOX_OBJECT_H
#define CPPLOX_OBJECT_H

#include <iostream>
#include <string>
#include <variant>

namespace cpplox {

    using Object = std::variant<std::monostate, std::string, double, bool>;

    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };

    template<class... Ts>
    overloaded(Ts...) -> overloaded<Ts...>;

    template<typename T, typename... Ts>
    std::ostream &operator<<(std::ostream &os, const std::variant<T, Ts...> &v) {
        std::visit(overloaded{
                           [&os](std::monostate) { os << "NULL"; },
                           [&os](bool arg) { os << (arg ? "TRUE" : "FALSE"); },
                           [&os](auto &&arg) { os << arg; }},
                   v);
        return os;
    }

}// namespace cpplox

#endif// CPPLOX_OBJECT_H
