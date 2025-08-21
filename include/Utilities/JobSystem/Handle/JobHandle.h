//
// Created by Oskar.Norberg on 2025-08-21.
//

#pragma once

#include <future>

namespace RNGOEngine::Utilities
{
    template<typename T>
    class JobHandle
    {
    public:
        JobHandle();
        explicit JobHandle(std::future<T>&& f);
        
        void Wait();

        T Get();

    private:
        std::future<T> m_future;
    };

#include "Utilities/JobSystem/Handle/JobHandle.tpp"
}