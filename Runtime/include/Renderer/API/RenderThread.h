//
// Created by Oskar.Norberg on 2025-11-27.
//

#pragma once

#include "Utilities/Threading/Runnable.h"

namespace RNGOEngine::Core::Renderer
{
    class RenderThread : public Utilities::Runnable
    {
    protected:
        void Initialize() override;
        void Step() override;
        void Shutdown() override;
    };
}