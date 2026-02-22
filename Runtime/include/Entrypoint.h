//
// Created by ringo on 2026-02-22.
//

#pragma once

#include <memory>

#include "Application/Application.h"

namespace RNGOEngine
{
    extern std::unique_ptr<Application> CreateApplication();

    inline int Main(const int argc, char** argv)
    {
        int success = 0;

        try
        {
            const auto app = CreateApplication();
            app->Run();
        }
        catch (FatalEngineError& e)
        {
            success = 1;
            RNGO_LOG(Core::LogLevel::Critical, "{}", e.what());
        }

        return success;
    }
}
