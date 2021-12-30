#ifndef CPPLOX_LOGGER_H
#define CPPLOX_LOGGER_H

#include <mutex>
#include <sstream>
#include <string>

namespace logger {
    enum Level {
        Info,
        Warning,
        Error,
        Debug,
        Trace,
        Null,
        num_levels
    };

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
        static Logger &getDefaultInstance();
        void log(Level lvl, const std::string &msg);
        void flush();

    private:
        FILE *fd;
        std::mutex mutex;
        std::array<LevelProps, Level::num_levels> lvls;
        explicit Logger(FILE *out = nullptr);
        ~Logger();
    };

    void info(const std::string &msg);
    void warning(const std::string &msg);
    void error(const std::string &msg);
    void debug(const std::string &msg);
    void trace(const std::string &msg);

    template<typename ...Args>
    void trace(const int line, const std::string &fileName, Args && ...args) {
        std::ostringstream stream;
        stream << fileName << "(" << line << ") : ";
        (stream << ... << std::forward<Args>(args));
        trace(stream.str());
    }
}// namespace logger


#endif//CPPLOX_LOGGER_H
