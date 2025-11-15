template<typename F, typename... Args>
auto ThreadPool::Enqueue(F&& f, Args&&... args) -> std::future<decltype(f(args...))>
{
    auto typedTask = std::make_shared<std::packaged_task<decltype(f(args...))()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    auto nonTypedTask = [typedTask]()
    {
        (*typedTask)();
    };

    {
        std::lock_guard<std::mutex> lock(mutex);
        taskQueue.push(nonTypedTask);
        condition.notify_one();
    }

    return typedTask->get_future();
}