
#include "AuraLogger.h"

namespace Aura {

    std::mutex Logger::s_Mutex;

    void Logger::Init() {
        // Could later set up file logging, filtering, etc.
    }

    void Logger::LogImpl(bool isCore, LogLevel level, const std::string& msg) {
        std::lock_guard<std::mutex> lock(s_Mutex);

        std::ostringstream output;
        output 
            << LevelToColor(level)
            << "[" << GetTimestamp() << "] "
            << "[" << (isCore ? "AURA ENGINE" : "AURA APP") << "] "
            << "[" << LevelToString(level) << "] "
            << msg 
            << "\033[0m"; // Reset color

        std::cout << output.str() << std::endl;
    }

    const char* Logger::LevelToString(LogLevel level) {
        switch (level) {
            case LogLevel::Trace: return "TRACE";
            case LogLevel::Info:  return "INFO";
            case LogLevel::Warn:  return "WARN";
            case LogLevel::Error: return "ERROR";
            case LogLevel::Fatal: return "FATAL";
            default: return "LOG";
        }
    }

    const char* Logger::LevelToColor(LogLevel level) {
        switch (level) {
            case LogLevel::Trace: return "\033[37m";     // White
            case LogLevel::Info:  return "\033[32m";     // Green
            case LogLevel::Warn:  return "\033[33m";     // Yellow
            case LogLevel::Error: return "\033[31m";     // Red
            case LogLevel::Fatal: return "\033[1;41m";   // Bold Red background
            default: return "\033[0m";                   // Reset
        }
    }

    std::string Logger::GetTimestamp() {
        using namespace std::chrono;
        auto now = system_clock::now();
        auto itt = system_clock::to_time_t(now);
        std::tm tm{};
    #if defined(_WIN32) || defined(_WIN64)
        localtime_s(&tm, &itt);
    #else
        localtime_r(&itt, &tm);
    #endif
        std::ostringstream ss;
        ss << std::put_time(&tm, "%H:%M:%S");
        return ss.str();
    }

}
