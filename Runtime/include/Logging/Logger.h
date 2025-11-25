//
// Created by Oskar.Norberg on 2025-11-25.
//

#pragma once

#include <spdlog/spdlog.h>

#include "Utilities/Singleton/Singleton.h"

namespace RNGOEngine::Core
{
    class Logger : public Utilities::Singleton<Logger>
    {
    public:
        Logger();
        ~Logger();

        void AttachSink(std::shared_ptr<spdlog::sinks::sink> sink);

    private:
        spdlog::logger m_logger;
    };
}
