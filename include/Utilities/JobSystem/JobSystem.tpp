template<typename F, typename ... Args>
auto JobSystem::Enqueue(F&& f, Args&&... args) -> JobHandle<decltype(f(args...))>
{
    auto future = m_threadPool.Enqueue(std::move(f), std::forward<Args>(args)...);
    return JobHandle<decltype(f(args...))>(std::move(future));
}