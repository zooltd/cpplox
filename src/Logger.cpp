#include "Logger.h"

logger::Logger::Logger(FILE *out)
    : fd(out) {
    lvls[Level::Info] = {"[ INFO    ]", blue};
    lvls[Level::Warning] = {"[ WARNING ]", yellow};
    lvls[Level::Error] = {"[ ERROR   ]", red};
    lvls[Level::Debug] = {"[ DEBUG   ]", green};
    lvls[Level::Trace] = {"[ TRACE   ]", reset};
    lvls[Level::Null] = {"", reset};
}

logger::Logger::~Logger() { fclose(fd); }

inline logger::Logger &logger::Logger::getDefaultInstance() {
    static Logger logger(stdout);
    return logger;
}


void logger::Logger::log(Level lvl, const std::string &msg) {
    std::lock_guard lock(mutex);
    // print Level
    fwrite(lvls[lvl].color.c_str(), sizeof(char), lvls[lvl].color.length() + 1, fd);
    fwrite(lvls[lvl].name.c_str(), sizeof(char), lvls[lvl].name.length() + 1, fd);
    fwrite(lvls[Level::Null].color.c_str(), sizeof(char), lvls[Level::Null].color.length() + 1, fd);
    // print msg
    fprintf(fd, " %s\n", msg.c_str());
}

inline void logger::Logger::flush() {
    std::lock_guard lock(mutex);
    fflush(fd);
}

inline void logger::info(const std::string &msg) {
    Logger::getDefaultInstance().log(Level::Info, msg);
    Logger::getDefaultInstance().flush();
}

inline void logger::warning(const std::string &msg) {
    Logger::getDefaultInstance().log(Level::Warning, msg);
    Logger::getDefaultInstance().flush();
}

inline void logger::error(const std::string &msg) {
    Logger::getDefaultInstance().log(Level::Error, msg);
    Logger::getDefaultInstance().flush();
}

inline void logger::debug(const std::string &msg) {
    Logger::getDefaultInstance().log(Level::Debug, msg);
    Logger::getDefaultInstance().flush();
}

void logger::trace(const std::string &msg) {
    Logger::getDefaultInstance().log(Level::Trace, msg);
    Logger::getDefaultInstance().flush();
}
