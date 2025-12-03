//
// Created by Oskar.Norberg on 2025-12-03.
//

#pragma once
#include <type_traits>

#include "spdlog/common.h"

namespace RNGOEngine::Core
{
    // This is bit-wised purely for filtering purposes.
    enum class LogLevel
    {
        Info = 1 << 0,
        Warning = 1 << 1,
        Error = 1 << 2,
        Critical = 1 << 3,
        Debug = 1 << 4,
    };
    
    constexpr LogLevel operator|(LogLevel a, LogLevel b)
    {
        return static_cast<LogLevel>(
            static_cast<std::underlying_type_t<LogLevel>>(a) |
            static_cast<std::underlying_type_t<LogLevel>>(b)
        );
    }
    constexpr LogLevel operator&(LogLevel a, LogLevel b)
    {
        return static_cast<LogLevel>(
            static_cast<std::underlying_type_t<LogLevel>>(a) &
            static_cast<std::underlying_type_t<LogLevel>>(b)
        );
    }
    constexpr LogLevel ALL_LEVELS =
        LogLevel::Info | LogLevel::Warning | LogLevel::Error | LogLevel::Critical | LogLevel::Debug;

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
}
