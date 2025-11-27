//
// Created by Oskar.Norberg on 2025-11-27.
//

#pragma once

#include <atomic>

namespace RNGOEngine::Utilities
{
    class Runnable
    {
    public:
        virtual ~Runnable() = default;

        virtual void Initialize(){}
        virtual void Shutdown(){}
        virtual void Step() = 0;

    public:
        void Run()
        {
            m_isRunning = true;

            Initialize();

            while (m_isRunning)
            {
                Step();
            }

            Shutdown();
        }

    public:
        void Stop()
        {
            m_isRunning = false;
        }

    private:
        std::atomic<bool> m_isRunning{false};
    };
}
