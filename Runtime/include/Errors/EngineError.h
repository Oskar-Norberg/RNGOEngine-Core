//
// Created by Oskar.Norberg on 2026-02-16.
//

#pragma once

#include <stdexcept>

#include "Logging/Logger.h"

#define RNGO_IRRECOVERABLE_ERROR(message) throw RNGOEngine::Errors::EngineError(message);

namespace RNGOEngine::Errors
{
    class EngineError : public std::runtime_error
    {
    public:
        explicit EngineError(const std::string& message)
            : runtime_error(message)
        {
            RNGO_LOG(Core::LogLevel::Error, "IRRECOVERABLE FAILURE: {}", message);
        }

        explicit EngineError(const char* string)
            : EngineError(std::string(string))
        {
        }

        explicit EngineError(std::string_view string)
            : EngineError(std::string(string))
        {
        }
    };
}