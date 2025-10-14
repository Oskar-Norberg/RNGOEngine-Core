#pragma once

template<typename T>
GenerationalKey<T> GenerationalVector<T>::Add(T&& data)
{
    if (!m_freeIndices.empty())
    {
        const auto index = m_freeIndices.back();
        m_freeIndices.pop_back();
        m_keys[index] = {m_keys[index].Generation + 1, true, std::move(data)};
        return {index, m_keys[index].Generation};
    }
    else
    {
        const auto index = m_keys.size();
        m_keys.emplace_back(0, true, std::move(data));
        return {index, 0};
    }
}

template<typename T>
void GenerationalVector<T>::MarkForRemoval(const GenerationalKey<T>& key)
{
    m_keys[key.ID].IsLive = false;
}

template<typename T>
void GenerationalVector<T>::Remove(const GenerationalKey<T>& key)
{
    m_freeIndices.emplace_back(key.ID);
    ++m_keys[key.ID].Generation;
}

template<typename T>
bool GenerationalVector<T>::IsValid(const GenerationalKey<T>& key) const
{
    return m_keys[key.ID].Generation == key.Generation && m_keys[key.ID].IsLive;
}

template<typename T>
const T& GenerationalVector<T>::Get(const GenerationalKey<T>& key) const
{
    // TODO: Redundant code for const and non-const ver
#ifndef N_DEBUG
    if (m_keys[key.ID].Generation != key.Generation || !m_keys[key.ID].IsLive)
    {
        RNGO_ASSERT(false && "Invalid Generational Key!");
    }
#endif

    return m_keys[key.ID].Data;
}

template<typename T>
T& GenerationalVector<T>::Get(const GenerationalKey<T>& key)
{
#ifndef N_DEBUG
    if (m_keys[key.ID].Generation != key.Generation || !m_keys[key.ID].IsLive)
    {
        RNGO_ASSERT(false && "Invalid Generational Key!");
    }
#endif

    return m_keys[key.ID].Data;
}