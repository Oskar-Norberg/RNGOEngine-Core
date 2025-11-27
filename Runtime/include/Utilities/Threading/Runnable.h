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

        // API (Non-Blocking)
    public:
        void Start();
        void Execute();
        void Stop();

        // API (Blocking)
    public:
        void Run();

        // Implementation
    protected:
        virtual void Initialize(){}
        virtual void Step() = 0;
        virtual void Shutdown(){}

    protected:
        std::atomic<bool> m_isRunning{false};
    };
}
