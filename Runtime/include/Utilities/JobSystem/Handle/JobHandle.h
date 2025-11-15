//
// Created by Oskar.Norberg on 2025-08-21.
//

#pragma once

#include <future>
#include <optional>

#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::Utilities
{
    // TODO: Rework these into "thin" handles that only store a JobID and a pointer to the JobSystem.
    template<typename T>
    class JobHandle
    {
    public:
        JobHandle();
        explicit JobHandle(std::future<T> future);
        
        void Wait();

        T Get();

    private:
        std::optional<std::future<T>> m_future;
    };

#include "Utilities/JobSystem/Handle/JobHandle.tpp"
}