template<typename T>
JobHandle<T>::JobHandle()
    : m_future(std::nullopt)
{
}

template<typename T>
JobHandle<T>::JobHandle(std::future<T> future)
    : m_future(std::optional(std::move(future)))
{
}

template<typename T>
void JobHandle<T>::Wait()
{
    RNGO_ASSERT(m_future.has_value() && "JobHandle::Wait called on an empty future.");
    RNGO_ZONE_SCOPED_N("JobHandle<T>::Wait");

    m_future.value().wait();
}

template<typename T>
T JobHandle<T>::Get()
{
    RNGO_ASSERT(m_future.has_value() && "JobHandle::Get called on an empty future.");
    RNGO_ZONE_SCOPED_N("JobHandle<T>::Get");

    return m_future.value().get();
}