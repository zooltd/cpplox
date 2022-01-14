#include "Environment.h"

#include "Meta.h"
#include <utility>

cpplox::Environment::Environment()
    : enclosing(nullptr) {}

cpplox::Environment::Environment(pEnv enclosing)
    : enclosing(std::move(enclosing)) {}

void cpplox::Environment::define(const std::string &name, Object value) { values[name] = std::move(value); }

cpplox::Object cpplox::Environment::get(const Token &name) {
    if (const auto v = values.find(name.lexeme); v != values.end()) return (*v).second;
    if (enclosing != nullptr) return enclosing->get(name);
    throw VarAccessErr(Meta::sourceFile, name.line, "Undefined variable '" + name.lexeme + "'.");
}

void cpplox::Environment::assign(const Token &name, Object value) {
    if (const auto v = values.find(name.lexeme); v != values.end()) {
        values[name.lexeme] = std::move(value);
        return;
    }
    if (enclosing != nullptr) {
        enclosing->assign(name, value);
        return;
    }
    throw VarAccessErr(Meta::sourceFile, name.line, "Undefined variable '" + name.lexeme + "'.");
}
