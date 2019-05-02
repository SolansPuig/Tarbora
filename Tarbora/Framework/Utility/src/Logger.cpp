#include "../inc/Logger.hpp"
#include <stdarg.h>
#include <ctime>

namespace Tarbora {
    bool Logger::Init(FILE *stream)
    {
        m_LogStream = stream;
        m_IsFile = false;
        return true;
    }

    bool Logger::Init(std::string file_path)
    {
        m_LogStream = fopen (file_path.c_str(), "w");
        m_IsFile = true;
        return m_LogStream != NULL;
    }

    void Logger::Close()
    {
        if (m_IsFile)
            fclose(m_LogStream);
    }

    void Logger::SetLevel(LogLevel level)
    {
        m_LogLevel = level;
    }

    void Logger::Log(LogLevel level, const char *text, ...)
    {
        // Only do something if the level is higher than the current log level.
        if (m_LogLevel <= level) {
            va_list args;
            va_start(args, text);

            // Print the current time
            time_t now = time(0);
            tm *ltm = localtime(&now);
            fprintf(m_LogStream, "[%02d:%02d:%02d]", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);

            // Print the Log Level colored. Does not work on windows.
            switch (level) {
                case LogLevel::DEBUG:
                    fprintf(m_LogStream, " \033[1;36mDEBUG:\033[0m ");
                    break;
                case LogLevel::INFO:
                    fprintf(m_LogStream, " \033[1;37mINFO:\033[0m  ");
                    break;
                case LogLevel::WARN:
                    fprintf(m_LogStream, " \033[1;93mWARN:\033[0m  ");
                    break;
                case LogLevel::ERR:
                    fprintf(m_LogStream, " \033[1;31mERR:\033[0m   ");
                    break;
            }

            // Print the message
            vfprintf(m_LogStream, text, args);
            va_end(args);
            fprintf(m_LogStream, "\n");
        }
    }
}
