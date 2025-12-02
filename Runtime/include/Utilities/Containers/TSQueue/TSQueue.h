//
// Created by Oskar.Norberg on 2025-12-01.
//

#pragma once
#include <mutex>
#include <queue>

#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::Containers
{
    template<typename T>
    class TSQueue
    {
    public:
        void Enqueue(T&& item)
        {
            std::lock_guard lock(m_mutex);
            m_queue.push(std::move(item));
        }

        bool IsEmpty()
        {
            std::lock_guard lock(m_mutex);
            return m_queue.empty();
        }

        T Dequeue()
        {
            std::lock_guard lock(m_mutex);
            if (m_queue.empty())
            {
                RNGO_ASSERT(false && "TSQueue::Dequeue called on empty queue.");
            }

            T item = std::move(m_queue.front());
            m_queue.pop();
            return item;
        }

    private:
        std::queue<T> m_queue;
        std::mutex m_mutex;
    };
}