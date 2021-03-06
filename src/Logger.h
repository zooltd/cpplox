#ifndef CPPLOX_LOGGER_H
#define CPPLOX_LOGGER_H

#include "Errors.h"
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

namespace logger {
    enum Level { Info,
                 Warning,
                 Error,
                 Debug,
                 Trace,
                 Null,
                 num_levels };

    struct LevelProps {
        std::string name;
        std::string color;
    };

    const std::string reset = "\033[m";
    const std::string blue = "\033[34m";
    const std::string red = "\033[31m";
    const std::string yellow = "\033[33m";
    const std::string green = "\033[32m";
    const std::string magenta = "\033[35m";
    const std::string cyan = "\033[36m";
    const std::string white = "\033[37m";

    class Logger {
    public:
        static Logger &getDefaultInstance() {
            static Logger logger(&std::cout);
            return logger;
        }

        template<typename T>
        void log(Level lvl, const T &msg) {
            std::lock_guard lock(mutex);
            *out << lvls[lvl].color << lvls[lvl].name << lvls[Level::Null].color << " "
                 << msg << std::endl;
        }

        void flush() {
            std::lock_guard lock(mutex);
            out->flush();
        }

    private:
        std::ostream *out;
        std::mutex mutex;
        std::array<LevelProps, Level::num_levels> lvls;
        explicit Logger(std::ostream *out) : out(out) {
            lvls[Level::Info] = {"[ INFO    ]", blue};
            lvls[Level::Warning] = {"[ WARNING ]", yellow};
            lvls[Level::Error] = {"[ ERROR   ]", red};
            lvls[Level::Debug] = {"[ DEBUG   ]", green};
            lvls[Level::Trace] = {"[ TRACE   ]", reset};
            lvls[Level::Null] = {"", reset};
        }
        ~Logger() = default;
    };

    template<typename T>
    void info(const T &msg) {
        Logger &l = Logger::getDefaultInstance();
        l.log(Level::Info, msg);
        l.flush();
    }

    template<typename T>
    void warning(const T &msg) {
        Logger &l = Logger::getDefaultInstance();
        l.log(Level::Warning, msg);
        l.flush();
    }

    template<typename T>
    void error(const T &msg) {
        Logger &l = Logger::getDefaultInstance();
        l.log(Level::Error, msg);
        l.flush();
    }

    template<typename T>
    void debug(const T &msg) {
        Logger &l = Logger::getDefaultInstance();
        l.log(Level::Debug, msg);
        l.flush();
    }

    template<typename T>
    void trace(const T &msg) {
        Logger &l = Logger::getDefaultInstance();
        l.log(Level::Trace, msg);
        l.flush();
    }

    template<typename... Args>
    void trace(const int line, const std::string &fileName, Args &&...args) {
        std::ostringstream stream;
        stream << fileName << "(" << line << ") : ";
        (stream << ... << std::forward<Args>(args));
        trace(stream.str());
    }

}// namespace logger

#endif// CPPLOX_LOGGER_H
