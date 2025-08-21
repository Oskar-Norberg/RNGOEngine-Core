//
// Created by Oskar.Norberg on 2025-08-20.
//

#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <queue>
#include <thread>
#include <vector>

namespace RNGOEngine::Utilities::ThreadPool
{
    class ThreadPool
    {
    public:
        explicit ThreadPool(size_t initialThreadCount = std::thread::hardware_concurrency());
        ~ThreadPool();

        template<typename F, typename... Args>
        auto Enqueue(F&& f, Args&&... args) -> std::future<decltype(f(args...))>;

    public:
        mutable std::mutex mutex;
        std::condition_variable condition;
        bool shouldStop;
        
        std::queue<std::function<void()>> taskQueue;
    
    private:
        std::vector<std::thread> m_threads;
    };

    void ThreadJob(ThreadPool& threadPool);

#include "Utilities/ThreadPool/ThreadPool.tpp"
}