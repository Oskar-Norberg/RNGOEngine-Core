//
// Created by Oskar.Norberg on 2025-08-20.
//

#include "Utilities/ThreadPool/ThreadPool.h"

RNGOEngine::Utilities::ThreadPool::ThreadPool::ThreadPool(size_t initialThreadCount)
    : shouldStop(false)
{
    m_threads.reserve(initialThreadCount);
    for (size_t i = 0; i < initialThreadCount; i++)
    {
        m_threads.emplace_back(ThreadJob, std::ref(*this));
    }
}

RNGOEngine::Utilities::ThreadPool::ThreadPool::~ThreadPool()
{
    shouldStop = true;
    condition.notify_all();

    for (auto& thread : m_threads)
    {
        thread.join();
    }
}

void RNGOEngine::Utilities::ThreadPool::ThreadJob(ThreadPool& threadPool)
{
    std::unique_lock<std::mutex> lock(threadPool.mutex);

    while (!threadPool.shouldStop)
    {
        if (!lock.owns_lock())
        {
            lock.lock();
        }

        threadPool.condition.wait(lock, [&threadPool]()
        {
            return !threadPool.taskQueue.empty() || threadPool.shouldStop;
        });

        if (!threadPool.taskQueue.empty())
        {
            if (threadPool.shouldStop)
                return;

            std::function<void()> task = std::move(threadPool.taskQueue.front());
            threadPool.taskQueue.pop();
            lock.unlock();

            task();
        }
    }
}