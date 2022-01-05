#ifndef CPPLOX_RUNNER_H
#define CPPLOX_RUNNER_H

#include <string>

namespace cpplox {
    class Runner {
    public:
        static int runScript(const std::string &filename);
        static int runREPL();

    private:
        static void run(const std::string &source);
    };
}// namespace cpplox

#endif//CPPLOX_RUNNER_H
