//
// Created by ringo on 2025-11-01.
//

#pragma once

#include <vector>

#include "Utilities/RNGOAsserts.h"
#include "Concepts/Concepts.h"

namespace RNGOEngine::Containers
{
    template<typename T>
    class FreeListVector
    {
    public:
        template<Concepts::ConstructibleFrom<T> U>
        size_t Insert(U&& data);

        void Remove(size_t index);

        T* Get(size_t index);

    private:
        struct Record
        {
            T data;
            bool isAlive = false;
        };

        std::vector<Record> m_data;
        std::vector<size_t> m_freeIndices;
    };

#include "FreeListVector.tpp"
}