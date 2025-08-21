template<typename T>
JobHandle<T>::JobHandle()
    : m_future()
{
}

template<typename T>
JobHandle<T>::JobHandle(std::future<T>&& f)
{
    m_future = std::move(f);
}

template<typename T>
void JobHandle<T>::Wait()
{
    m_future.wait();
}

template<typename T>
T JobHandle<T>::Get()
{
    return m_future.get();
}