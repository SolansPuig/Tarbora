#include "Logger.hpp"
#include <stdarg.h>
#include <ctime>

namespace Tarbora {
    namespace Logger {
        LogLevel log_level;
        FILE *log_stream;
        bool isFile;

        void Init(FILE *stream)
        {
            log_stream = stream;
            isFile = false;
        }

        void Init(const char *file_path)
        {
            log_stream = fopen (file_path, "w");
            isFile = true;
        }

        void Close()
        {
            if (isFile)
                fclose(log_stream);
        }

        void SetLevel(LogLevel level)
        {
            log_level = level;
        }

        void Log(LogLevel level, const char *text, ...)
        {
            if (log_level <= level) {
                va_list args;
                va_start(args, text);

                // Print the current time
                time_t now = time(0);
                tm *ltm = localtime(&now);
                fprintf(log_stream, "[%02d:%02d:%02d]", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);

                // Print the Log Level
                switch (level) {
                    case LogLevel::DEBUG:
                        fprintf(log_stream, " \033[1;36mDEBUG:\033[0m ");
                        break;
                    case LogLevel::INFO:
                        fprintf(log_stream, " \033[1;37mINFO:\033[0m  ");
                        break;
                    case LogLevel::WARN:
                        fprintf(log_stream, " \033[1;93mWARN:\033[0m  ");
                        break;
                    case LogLevel::ERR:
                        fprintf(log_stream, " \033[1;31mERR:\033[0m   ");
                        break;
                }

                // Print the message
                vfprintf(log_stream, text, args);
                va_end(args);
                fprintf(log_stream, "\n");
            }
        }
    }
}
