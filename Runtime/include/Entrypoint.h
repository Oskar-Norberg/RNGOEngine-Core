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
        const auto application = CreateApplication();
        application->Run();

        return 0;
    }
}
