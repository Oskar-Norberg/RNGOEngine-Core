class MarkedGenerationalIterator
{
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = GenerationalKey<T>;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

    MarkedGenerationalIterator(const GenerationalVector<T>* vec, size_t index)
        : m_vec(vec), m_index(index)
    {
        AdvanceToNextValid();
    }

    value_type operator*() const
    {
        return GenerationalKey<T>{m_index, m_vec->m_keys[m_index].Generation};
    }

    MarkedGenerationalIterator& operator++()
    {
        ++m_index;
        AdvanceToNextValid();
        return *this;
    }

    bool operator==(const MarkedGenerationalIterator& other) const
    {
        return m_index == other.m_index && m_vec == other.m_vec;
    }

    bool operator!=(const MarkedGenerationalIterator& other) const
    {
        return !(*this == other);
    }

private:
    void AdvanceToNextValid()
    {
        while (m_index < m_vec->m_keys.size() && m_vec->m_keys[m_index].Status != GenerationalKeyStatus::Marked)
        {
            ++m_index;
        }
    }

    const GenerationalVector<T>* m_vec;
    size_t m_index;
};

struct MarkedRange
{
    MarkedGenerationalIterator beginIt;
    MarkedGenerationalIterator endIt;

    auto begin() const
    {
        return beginIt;
    }

    auto begin()
    {
        return beginIt;
    }

    auto end() const
    {
        return endIt;
    }

    auto end()
    {
        return endIt;
    }
};