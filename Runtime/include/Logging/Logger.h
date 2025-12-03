//
// Created by Oskar.Norberg on 2025-11-25.
//

#pragma once

#include <spdlog/spdlog.h>

#include <magic_enum/magic_enum.hpp>
#include <source_location>

#include "Utilities/Singleton/Singleton.h"

namespace RNGOEngine::Core
{
    enum class LogLevel
    {
        Info,
        Warning,
        Error,
        Critical,
        Debug
    };

    constexpr LogLevel SPDLogLevelToRNGOLevel(spdlog::level::level_enum level)
    {
        switch (level)
        {
            case spdlog::level::debug:
                return LogLevel::Debug;
            case spdlog::level::warn:
                return LogLevel::Warning;
            case spdlog::level::err:
                return LogLevel::Error;
            case spdlog::level::critical:
                return LogLevel::Critical;
            case spdlog::level::info:
            default:
                return LogLevel::Info;
        }
    }

    constexpr spdlog::level::level_enum RNGOLevelToSPDLogLevel(LogLevel level)
    {
        switch (level)
        {
            case LogLevel::Debug:
                return spdlog::level::debug;
            case LogLevel::Warning:
                return spdlog::level::warn;
            case LogLevel::Error:
                return spdlog::level::err;
            case LogLevel::Critical:
                return spdlog::level::critical;
            case LogLevel::Info:
            default:
                return spdlog::level::info;
        }
    }

    struct LogLocation
    {
        std::string_view File;
        std::string_view Function;
        size_t Line;
    };

    struct LogEntry
    {
        LogLevel Level;
        std::string Message;
        LogLocation Location;
        std::chrono::time_point<std::chrono::system_clock> TimePoint;
    };

    class Logger : public Utilities::Singleton<Logger>
    {
    public:
        Logger();
        ~Logger();

    public:
        template<typename... Args>
        static void Log(
            std::source_location location, const LogLevel level, spdlog::format_string_t<Args...> fmt,
            Args&&... args
        )
        {
            spdlog::source_loc loc{
                location.file_name(),
                static_cast<int>(location.line()),
                location.function_name()
            };
            
            // TODO: What happens if logger is not yet initialized?
            auto& logger = GetInstance().m_logger;

            const auto logLevel = RNGOLevelToSPDLogLevel(level);
            logger.log(loc, logLevel, fmt, std::forward<Args>(args)...);
        }

    public:
        void AttachSink(std::shared_ptr<spdlog::sinks::sink> sink);

    private:
        // TODO: Should this be moved to a static context? To avoid having to go through the singleton every time?
        spdlog::logger m_logger;
    };
}

#define RNGO_LOG(level, ...) RNGOEngine::Core::Logger::Log(std::source_location::current(), level, __VA_ARGS__)