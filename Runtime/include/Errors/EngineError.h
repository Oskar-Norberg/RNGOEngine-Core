//
// Created by Oskar.Norberg on 2026-02-16.
//

#pragma once

#include <stdexcept>

#define RNGO_FATAL_ERROR(Message) throw FatalEngineError(Message);

namespace RNGOEngine
{
    class EngineError : public std::runtime_error
    {
    public:
        explicit EngineError(const std::string& message)
            : runtime_error(message)
        {
        }

        explicit EngineError(const char* message)
            : EngineError(std::string{message})
        {
        }

        explicit EngineError(const std::string_view message)
            : EngineError(std::string{message})
        {
        }
    };

    class FatalEngineError : public EngineError
    {
    public:
        explicit FatalEngineError(const std::string& message)
            : EngineError(message)
        {
        }

        explicit FatalEngineError(const char* message)
            : FatalEngineError(std::string{message})
        {
        }

        explicit FatalEngineError(const std::string_view message)
            : FatalEngineError(std::string{message})
        {
        }
    };
}
