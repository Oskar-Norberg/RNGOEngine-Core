//
// Created by Oskar.Norberg on 2025-11-27.
//

#include "Utilities/Threading/Runnable.h"

namespace RNGOEngine::Utilities
{
    void Runnable::Start()
    {
        m_isRunning = true;
        Initialize();
    }

    void Runnable::Execute()
    {
        Step();
    }

    void Runnable::Stop()
    {
        m_isRunning = false;
        Shutdown();
    }

    void Runnable::Run()
    {
        m_isRunning = true;
        Initialize();
        while (m_isRunning)
        {
            Step();
        }

        Shutdown();
        m_isRunning = false;
    }
}