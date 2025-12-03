//
// Created by Oskar.Norberg on 2025-11-25.
//

#pragma once

#include <spdlog/spdlog.h>

#include <magic_enum/magic_enum.hpp>
#include <source_location>

#include "LogLevel.h"
#include "Utilities/Singleton/Singleton.h"

namespace RNGOEngine::Core
{
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
                location.file_name(), static_cast<int>(location.line()), location.function_name()
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