#pragma once
#include <stdio.h>

namespace Tarbora {
    namespace Logger {
        enum LogLevel { DEBUG=0, INFO, WARN, ERR };

        void Init(FILE *stream);
        void Init(const char *file_path);
        void Close();

        void SetLevel(LogLevel level);
        void Log(LogLevel level, const char *text, ...);
    }
}

#define LOG_LEVEL(L) ::Tarbora::Logger::SetLevel(L)
#define LOG_DEBUG(...) ::Tarbora::Logger::Log(::Tarbora::Logger::LogLevel::DEBUG, __VA_ARGS__)
#define LOG_INFO(...) ::Tarbora::Logger::Log(::Tarbora::Logger::LogLevel::INFO, __VA_ARGS__)
#define LOG_WARN(...) ::Tarbora::Logger::Log(::Tarbora::Logger::LogLevel::WARN, __VA_ARGS__)
#define LOG_ERR(...) ::Tarbora::Logger::Log(::Tarbora::Logger::LogLevel::ERR, __VA_ARGS__)
