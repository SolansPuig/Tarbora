#pragma once

#include "CommonHeaders.hpp"

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
        static void setLevel(LogLevel level) { log_level_ = level; }

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
        static void log(LogLevel level, const char *text, ...);

    private:
        inline static LogLevel log_level_; // The current log level.
    };
}

#define LOG_LEVEL(LEVEL) ::Tarbora::Logger::setLevel(::Tarbora::Logger::LogLevel::LEVEL)
#define LOG_DEBUG(...) ::Tarbora::Logger::log(::Tarbora::Logger::LogLevel::DEBUG, __VA_ARGS__)
#define LOG_INFO(...) ::Tarbora::Logger::log(::Tarbora::Logger::LogLevel::INFO, __VA_ARGS__)
#define LOG_WARN(...) ::Tarbora::Logger::log(::Tarbora::Logger::LogLevel::WARN, __VA_ARGS__)
#define LOG_ERR(...) ::Tarbora::Logger::log(::Tarbora::Logger::LogLevel::ERR, __VA_ARGS__)
