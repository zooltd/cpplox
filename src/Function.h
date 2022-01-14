#ifndef CPPLOX_FUNCTION_H
#define CPPLOX_FUNCTION_H

#include <vector>

namespace cpplox {

    class Callable;

    class Clock : public Callable {
    public:
        int arity() override { return 0; }

        Object call(Interpreter &interpreter, const std::vector<Object> &arguments) override {
            return static_cast<double>(
                std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::high_resolution_clock::now().time_since_epoch()).count());
        }

        std::string toString() override { return "<native fn>"; }
    };

    class Function : public Callable {
    public:
        explicit Function(const AST::pFunctionStmt &declaration)
            : declaration(declaration) {}

        int arity() override { return static_cast<int>(declaration->params.size()); }

        Object call(Interpreter &interpreter, const std::vector<Object> &arguments) override {
            const pEnv env{interpreter.globals};
            for (int i = 0; i < arity(); i++) { env->define(declaration->params[i].lexeme, arguments[i]); }
            interpreter.executeBlock(declaration->body, env);
            return Object{};
        }

        std::string toString() override { return "<fn " + declaration->name.lexeme + ">"; }


    private:
        const AST::pFunctionStmt &declaration;
    };


}// namespace cpplox

#endif// CPPLOX_FUNCTION_H
