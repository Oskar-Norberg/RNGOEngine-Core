//
// Created by Oskar.Norberg on 2025-12-03.
//

#pragma once
#include <type_traits>

#include "Utilities/EnumDefinitions.h"
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
        All = Info | Warning | Error | Critical | Debug,
    };
    DEFINE_ENUM_CLASS_BITWISE_OPERATORS(LogLevel);
    

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
