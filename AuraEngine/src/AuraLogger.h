
#pragma once
#include <iostream>
#include <sstream>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <format> // C++20 formatting
#include "AuraAPI.h"

namespace Aura {

    enum class LogLevel {
        Trace, Info, Warn, Error, Fatal
    };

    class AURA_API Logger {
    public:
        static void Init();
        
        template<typename... Args>
        static void LogCore(LogLevel level, std::string_view fmt, Args&&... args) {
            LogImpl(true, level, std::vformat(fmt, std::make_format_args(args...)));
        }

        template<typename... Args>
        static void LogApp(LogLevel level, std::string_view fmt, Args&&... args) {
            LogImpl(false, level, std::vformat(fmt, std::make_format_args(args...)));
        }

    private:
        static void LogImpl(bool isCore, LogLevel level, const std::string& msg);
        static std::mutex s_Mutex;
        static const char* LevelToString(LogLevel level);
        static const char* LevelToColor(LogLevel level);
        static std::string GetTimestamp();

    };

} // namespace Aura

// ===== Macros for Core (Engine) logs =====
#define AURA_CORE_TRACE(fmt, ...)  ::Aura::Logger::LogCore(::Aura::LogLevel::Trace, fmt, ##__VA_ARGS__)
#define AURA_CORE_INFO(fmt, ...)   ::Aura::Logger::LogCore(::Aura::LogLevel::Info,  fmt, ##__VA_ARGS__)
#define AURA_CORE_WARN(fmt, ...)   ::Aura::Logger::LogCore(::Aura::LogLevel::Warn,  fmt, ##__VA_ARGS__)
#define AURA_CORE_ERROR(fmt, ...)  ::Aura::Logger::LogCore(::Aura::LogLevel::Error, fmt, ##__VA_ARGS__)
#define AURA_CORE_FATAL(fmt, ...)  ::Aura::Logger::LogCore(::Aura::LogLevel::Fatal, fmt, ##__VA_ARGS__)

// ===== Macros for App (Client/Game) logs =====
#define AURA_TRACE(fmt, ...)       ::Aura::Logger::LogApp(::Aura::LogLevel::Trace, fmt, ##__VA_ARGS__)
#define AURA_INFO(fmt, ...)        ::Aura::Logger::LogApp(::Aura::LogLevel::Info,  fmt, ##__VA_ARGS__)
#define AURA_WARN(fmt, ...)        ::Aura::Logger::LogApp(::Aura::LogLevel::Warn,  fmt, ##__VA_ARGS__)
#define AURA_ERROR(fmt, ...)       ::Aura::Logger::LogApp(::Aura::LogLevel::Error, fmt, ##__VA_ARGS__)
#define AURA_FATAL(fmt, ...)       ::Aura::Logger::LogApp(::Aura::LogLevel::Fatal, fmt, ##__VA_ARGS__)
