#include "Runner.h"
#include "Interpreter.h"
#include "Logger.h"
#include "Meta.h"
#include "Parser.h"
#include "Scanner.h"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <sysexits.h>

static cpplox::Interpreter interpreter;

int cpplox::Runner::runScript(const std::string &filename) {
    Meta::sourceFile = filename;
    const std::string source = [&]() -> std::string {
        try {
            std::ifstream f(filename, std::ios::in);
            f.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            std::string content{std::istreambuf_iterator(f),
                                std::istreambuf_iterator<char>()};
            return content;
        } catch (std::exception &e) {
            std::ostringstream stream;
            stream << "Couldn't open input source file (" << e.what() << ").";
            logger::trace(__LINE__, __FILE__, stream.str());
            return "";
        }
    }();

    if (source.empty())
        return EX_DATAERR;

    run(source);

    if (Errors::hadError)
        return EX_DATAERR;
    if (Errors::hadRuntimeError)
        return EX_SOFTWARE;
    return 0;
}

int cpplox::Runner::runREPL() {
    std::string line;
    const auto in_time_t =
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << "cpplox v1.0.0 ("
              << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X") << ")"
              << std::endl;
    std::cout << std::endl;
    std::cout << R"(Type "help" for more information.)" << std::endl;

    while (std::cout << ">>> " && std::getline(std::cin, line)) {
        run(line);
        Errors::hadError = false;
    }
    std::cout << std::endl
              << "Goodbye!" << std::endl;
    return 0;
}

void cpplox::Runner::run(const std::string &source) {
    Scanner scanner(source);
    const std::vector<Token> tokens = scanner.scanTokens();
    // For now, just print the tokens.
    for (const Token &token: tokens) {
        std::cout << token << std::endl;
    }

    Parser parser(tokens);
    const auto expression = parser.parse();
    // Stop if there was a syntax error.
    if (Errors::hadError)
        return;
    interpreter.interpret(expression);
}
