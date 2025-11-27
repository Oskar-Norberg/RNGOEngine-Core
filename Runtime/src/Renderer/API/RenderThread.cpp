//
// Created by Oskar.Norberg on 2025-11-27.
//

#include "Renderer/API/RenderThread.h"

namespace RNGOEngine::Core::Renderer
{
    void RenderThread::Initialize()
    {
        Runnable::Initialize();
    }

    void RenderThread::Step()
    {
    }

    void RenderThread::Shutdown()
    {
        Runnable::Shutdown();
    }
}