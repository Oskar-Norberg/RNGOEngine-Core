#pragma once

template<typename T>
template<typename U>
GenerationalKey<T> GenerationalVector<T>::Insert(U&& data)
{
    if (!m_freeIndices.empty())
    {
        const auto index = m_freeIndices.size() - 1;
        m_freeIndices.pop_back();
        m_keys[index] = InternalGenerationalKey<T>{m_keys[index].Generation + 1, true, std::forward<U>(data)};
        return {index, m_keys[index].Generation};
    }
    else
    {
        const auto index = m_keys.size();
        m_keys.push_back(InternalGenerationalKey<T>{0, true, std::forward<U>(data)});
        return {index, 0};
    }
}

template<typename T>
void GenerationalVector<T>::MarkForRemoval(const GenerationalKey<T>& key)
{
    if (IsValid(key))
    {
        m_keys[key.ID].IsLive = false;
    }
    else
    {
        RNGO_ASSERT(false && "GenerationalVector<T>::MarkForRemoval called with invalid key");
    }
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
    if (!IsValid(key))
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
    if (!IsValid(key))
    {
        RNGO_ASSERT(false && "Invalid Generational Key!");
    }
#endif

    return m_keys[key.ID].Data;
}

template<typename T>
std::optional<std::reference_wrapper<const T>> GenerationalVector<T>::GetValidated(
    const GenerationalKey<T>& key) const
{
    if (!IsValid(key))
    {
        return std::nullopt;
    }

    return std::cref(Get(key));
}

template<typename T>
std::optional<std::reference_wrapper<T>> GenerationalVector<T>::GetValidated(
    const GenerationalKey<T>& key)
{
    if (!IsValid(key))
    {
        return std::nullopt;
    }

    return std::ref(Get(key));
}