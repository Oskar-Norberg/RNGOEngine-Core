//
// Created by Oskar.Norberg on 2025-08-21.
//

#pragma once

#include "Handle/JobHandle.h"
#include "Utilities/ThreadPool/ThreadPool.h"

namespace RNGOEngine::Utilities
{
    class JobSystem
    {
    public:
        template<typename F, typename... Args>
        auto Enqueue(F&& f, Args&&... args) -> JobHandle<decltype(f(args...))>;

    private:
        ThreadPool::ThreadPool m_threadPool;
    };

#include "Utilities/JobSystem/JobSystem.tpp"
}