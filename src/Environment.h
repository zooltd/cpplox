#ifndef CPPLOX_ENVIRONMENT_H
#define CPPLOX_ENVIRONMENT_H

#include "Object.h"
#include "Token.h"
#include "Errors.h"
#include <string>
#include <unordered_map>
#include <memory>

namespace cpplox {

    class Environment;

    using VarAccessErr = Errors::Err;

    using pEnv = std::shared_ptr<Environment>;

    class Environment : std::enable_shared_from_this<Environment> {
    public:
        // a reference to its enclosing one
        pEnv enclosing;

        Environment();
        explicit Environment(pEnv enclosing);

        // binds a new name to a value
        void define(const std::string &name, Object value);
        Object get(const Token &name);
        // throws a runtime error if the key doesn¡¯t already exist in the environment¡¯s variable map
        void assign(const Token &name, Object value);

    private:
        std::unordered_map<std::string, Object> values;
    };
}// namespace cpplox

#endif//CPPLOX_ENVIRONMENT_H
