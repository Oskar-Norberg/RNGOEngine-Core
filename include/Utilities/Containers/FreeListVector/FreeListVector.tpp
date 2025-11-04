template<typename T>
template<Concepts::ConstructibleFrom<T> U>
size_t FreeListVector<T>::Insert(U&& data)
{
    if (m_freeIndices.empty())
    {
        m_data.push_back(Record{std::forward<U>(data), true});
        m_data.back().isAlive = true;
        return m_data.size() - 1;
    }
    else
    {
        const auto freeIndex = m_freeIndices.back();
        m_freeIndices.pop_back();
        m_data[freeIndex].data = std::forward<U>(data);
        m_data[freeIndex].isAlive = true;

        return freeIndex;
    }
}

template<typename T>
void FreeListVector<T>::Remove(size_t index)
{
    RNGO_ASSERT(index < m_data.size());
    if (!m_data[index].isAlive)
    {
        RNGO_ASSERT(false && "FreeListVector::Remove called on dead index.");
        return;
    }

    m_freeIndices.push_back(index);
    m_data[index].isAlive = false;
}

template<typename T>
T* FreeListVector<T>::Get(size_t index)
{
    if (index >= m_data.size() || !m_data[index].isAlive)
    {
        return nullptr;
    }

    return &m_data[index].data;
}