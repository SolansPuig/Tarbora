#pragma once
#include <stdio.h>
#include <string>

namespace Tarbora {
    class Logger {
    public:
        enum LogLevel { DEBUG=0, INFO, WARN, ERR };

        //! Set the log level.
        /*!
            \param level Levels lower than that will be ignored.

            It can be called through a macro:
            \code{.cpp}
                LOG_LEVEL(LEVEL)
            \endcode
        */
        static void SetLevel(LogLevel level) { s_LogLevel = level; }

        //! Log a message.
        /*!
            \param level The log level of the message.
            \param text The message itself, formatted as a printf.
            \param ... The extra params of the printf.

            It can be called through the macros:
            \code{.cpp}
                LOG_DEBUG(TEXT, ...)
                LOG_INFO(TEXT, ...)
                LOG_WARN(TEXT, ...)
                LOG_ERR(TEXT, ...)
            \endcode
        */
        static void Log(LogLevel level, const char *text, ...);

    private:
        inline static LogLevel s_LogLevel; // The current log level.
    };
}

#define LOG_LEVEL(LEVEL) ::Tarbora::Logger::SetLevel(::Tarbora::Logger::LogLevel::LEVEL)
#define LOG_DEBUG(...) ::Tarbora::Logger::Log(::Tarbora::Logger::LogLevel::DEBUG, __VA_ARGS__)
#define LOG_INFO(...) ::Tarbora::Logger::Log(::Tarbora::Logger::LogLevel::INFO, __VA_ARGS__)
#define LOG_WARN(...) ::Tarbora::Logger::Log(::Tarbora::Logger::LogLevel::WARN, __VA_ARGS__)
#define LOG_ERR(...) ::Tarbora::Logger::Log(::Tarbora::Logger::LogLevel::ERR, __VA_ARGS__)
