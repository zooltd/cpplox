#include "Environment.h"

#include "Meta.h"
#include <utility>

void cpplox::Environment::define(const std::string &name, cpplox::Object value) {
    values[name] = std::move(value);
}

cpplox::Object cpplox::Environment::get(const cpplox::Token &name) {
    auto iter = values.find(name.lexeme);
    if (iter != values.end())
        return (*iter).second;
    throw VarAccessErr(Meta::sourceFile, name.line, "Undefined variable '" + name.lexeme + "'.");
}
