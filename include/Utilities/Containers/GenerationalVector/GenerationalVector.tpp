#pragma once

template<typename T>
GenerationalKey GenerationalVector<T>::Add(T&& data)
{
    if (!m_freeIndices.empty())
    {
        const auto index = m_freeIndices.back();
        m_freeIndices.pop_back();
        ++m_keys[index].Generation;
        m_keys[index].Data = std::move(data);
        return {index, m_keys[index].Generation};
    }
    else
    {
        const auto index = m_keys.size();
        m_keys.emplace_back(index, 0, std::move(data));
        return {index, 0};
    }
}

template<typename T>
void GenerationalVector<T>::Remove(const GenerationalKey& key)
{
    m_freeIndices.emplace_back(key.ID);
    ++m_keys[key.ID].Generation;
}

template<typename T>
bool GenerationalVector<T>::IsValid(const GenerationalKey& key) const
{
    return m_keys[key.ID].Generation == key.Generation;
}

template<typename T>
const T& GenerationalVector<T>::Get(const GenerationalKey& key) const
{
    // TODO: Redundant code for const and non-const ver
#ifndef N_DEBUG
    if (m_keys[key.ID].Generation != key.Generation)
    {
        RNGO_ASSERT(false && "Invalid Generational Key!");
    }
#endif

    return m_keys[key.ID].Data;
}

template<typename T>
T& GenerationalVector<T>::Get(const GenerationalKey& key)
{
#ifndef N_DEBUG
    if (m_keys[key.ID].Generation != key.Generation)
    {
        RNGO_ASSERT(false && "Invalid Generational Key!");
    }
#endif

    return m_keys[key.ID].Data;
}