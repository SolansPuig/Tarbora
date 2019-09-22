#include "Logger.hpp"

namespace Tarbora {
    void Logger::Log(LogLevel level, const char *text, ...) {
        // Only do something if the level is higher than the current log level.
        if (s_LogLevel <= level) {
            va_list args;
            va_start(args, text);

            // Print the current time
            time_t now = time(0);
            tm *ltm = localtime(&now);
            fprintf(stdout, "[%02d:%02d:%02d]", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);

            // Print the Log Level colored. Does not work on windows.
            switch (level) {
                case LogLevel::DEBUG:
                    fprintf(stdout, " \033[1;36mDEBUG:\033[0m ");
                    break;
                case LogLevel::INFO:
                    fprintf(stdout, " \033[1;37mINFO:\033[0m  ");
                    break;
                case LogLevel::WARN:
                    fprintf(stdout, " \033[1;93mWARN:\033[0m  ");
                    break;
                case LogLevel::ERR:
                    fprintf(stdout, " \033[1;31mERR:\033[0m   ");
                    break;
            }

            // Print the message
            vfprintf(stdout, text, args);
            va_end(args);
            fprintf(stdout, "\n");
        }
    }
}
