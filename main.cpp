#include "Runner.h"
#include <iostream>
#include <sysexits.h>

int main(const int argc, char **argv) {
    if (argc == 1) return cpplox::Runner::runREPL();
    if (argc == 2) return cpplox::Runner::runScript(argv[1]);
    std::cout << "Usage: cpplox [script]" << std::endl;
    return EX_USAGE;
}
