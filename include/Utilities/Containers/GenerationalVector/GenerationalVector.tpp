template<typename T>
template<Concepts::ConstructibleFrom<T> U>
GenerationalKey<T> GenerationalVector<T>::Insert(U&& data)
{
    if (!m_freeIndices.empty())
    {
        const auto index = m_freeIndices.back();
        m_freeIndices.pop_back();
        m_keys[index] = InternalGenerationalKey<T>{m_keys[index].Generation,
                                                   GenerationalKeyStatus::Unmarked, std::forward<U>(data)};
        return {index, m_keys[index].Generation};
    }
    else
    {
        const auto index = m_keys.size();
        m_keys.push_back(
            InternalGenerationalKey<T>{0, GenerationalKeyStatus::Unmarked, std::forward<U>(data)});
        return {index, 0};
    }
}

template<typename T>
void GenerationalVector<T>::MarkForRemoval(const GenerationalKey<T>& key)
{
    if (IsValidUnmarked(key))
    {
        m_keys[key.ID].Status = GenerationalKeyStatus::Marked;
    }
    else
    {
        RNGO_ASSERT(false && "GenerationalVector<T>::MarkForRemoval called with invalid key");
    }
}

template<typename T>
void GenerationalVector<T>::Remove(const GenerationalKey<T>& key)
{
    m_keys[key.ID].Status = GenerationalKeyStatus::Removed;
    m_freeIndices.emplace_back(key.ID);
    ++m_keys[key.ID].Generation;
}

template<typename T>
bool GenerationalVector<T>::IsValidUnmarked(const GenerationalKey<T>& key) const
{
    return IsValidID(key) && MatchesGeneration(key) && IsUnmarked(key);
}

template<typename T>
bool GenerationalVector<T>::IsValidMarked(const GenerationalKey<T>& key) const
{
    return IsValidID(key) && MatchesGeneration(key) && IsMarked(key);
}

template<typename T>
const T& GenerationalVector<T>::GetUnmarked(const GenerationalKey<T>& key) const
{
    if (!IsValidUnmarked(key))
    {
        RNGO_ASSERT(false && "Invalid Generational Key!");
    }

    return m_keys[key.ID].Data;
}

template<typename T>
T& GenerationalVector<T>::GetUnmarked(const GenerationalKey<T>& key)
{
    return const_cast<T&>(static_cast<const GenerationalVector*>(this)->GetUnmarked(key));
}

template<typename T>
std::optional<std::reference_wrapper<const T>> GenerationalVector<T>::GetUnmarkedValidated(
    const GenerationalKey<T>& key) const
{
    if (!IsValidUnmarked(key))
    {
        return std::nullopt;
    }

    return std::cref(GetUnmarked(key));
}

template<typename T>
std::optional<std::reference_wrapper<T>> GenerationalVector<T>::GetUnmarkedValidated(
    const GenerationalKey<T>& key)
{
    auto unmarkedOpt = static_cast<const GenerationalVector<T>*>(this)->GetUnmarkedValidated(key);
    if (!unmarkedOpt)
    {
        return std::nullopt;
    }

    return std::ref(const_cast<T&>(unmarkedOpt.value().get()));
}

template<typename T>
T& GenerationalVector<T>::GetMarked(const GenerationalKey<T>& key)
{
    if (!IsValidMarked(key))
    {
        RNGO_ASSERT(false && "Invalid Generational Key!");
    }

    return m_keys[key.ID].Data;
}

template<typename T>
const T& GenerationalVector<T>::GetMarked(const GenerationalKey<T>& key) const
{
    return const_cast<T&>(static_cast<const GenerationalVector*>(this)->GetMarked(key));
}

template<typename T>
std::optional<std::reference_wrapper<T>> GenerationalVector<T>::GetMarkedValidated(
    const GenerationalKey<T>& key)
{
    if (!IsValidMarked(key))
    {
        return std::nullopt;
    }

    return std::cref(Get(key));
}

template<typename T>
std::optional<std::reference_wrapper<const T>> GenerationalVector<T>::GetMarkedValidated(
    const GenerationalKey<T>& key) const
{
    const auto markedOption = GetMarkedValidated(key);
    if (!markedOption)
    {
        return markedOption;
    }

    return std::ref(const_cast<T&>(markedOption.value().get()));
}

template<typename T>
bool GenerationalVector<T>::IsValidID(const GenerationalKey<T>& key) const
{
    return m_keys.size() > key.ID;
}

template<typename T>
bool GenerationalVector<T>::MatchesGeneration(const GenerationalKey<T>& key) const
{
    // Assumes ID is a valid index.
    return m_keys[key.ID].Generation == key.Generation;
}

template<typename T>
bool GenerationalVector<T>::IsUnmarked(const GenerationalKey<T>& key) const
{
    // Assumes key is valid.
    return m_keys[key.ID].Status == GenerationalKeyStatus::Unmarked;
}

template<typename T>
bool GenerationalVector<T>::IsMarked(const GenerationalKey<T>& key) const
{
    return m_keys[key.ID].Status == GenerationalKeyStatus::Marked;
}