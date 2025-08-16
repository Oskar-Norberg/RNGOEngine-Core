template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::AddNode(T data, const Math::BoundingBox& bounds)
{
    totalCapacity++;

    if (m_dataIndex >= CAPACITY && !IsSubdivided())
    {
        Subdivide();
    }

    if (IsSubdivided())
    {
        for (const auto& subTree : m_subTrees)
        {
            if (subTree->m_boundingBox.Intersects(bounds))
            {
                subTree->AddNode(data, bounds);
            }
        }
    }
    else
    {
        // Add to overflow if it doesn't fit entirely.
        if (m_boundingBox.Contains(bounds))
        {
            m_data[m_dataIndex].data = data;
            m_data[m_dataIndex].bounds = bounds;
            m_dataIndex++;
        }
        else
        {
            m_overflowData.emplace_back(data, bounds);
        }
    }
}

template<typename T, size_t CAPACITY>
std::unordered_set<std::pair<T, T>, Utilities::Hash::PairHash> QuadTree<T, CAPACITY>::GetCollisionPairs() const
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::GetCollisionPairs");

    std::unordered_set<std::pair<T, T>, Utilities::Hash::PairHash> collisionPairs;

    std::stack<const QuadTree*> stack;
    stack.emplace(this);

    while (!stack.empty())
    {
        const QuadTree* currentTree = std::move(stack.top());
        stack.pop();

        for (size_t i = 0; i < currentTree->m_dataIndex; ++i)
        {
            // Tree - Tree collisions
            for (size_t j = i + 1; j < currentTree->m_dataIndex; ++j)
            {
                if (currentTree->m_data[i].bounds.Intersects(currentTree->m_data[j].bounds))
                {
                    const auto idA = currentTree->m_data[i].data;
                    const auto idB = currentTree->m_data[j].data;

                    collisionPairs.insert(std::minmax(idA, idB));
                }
            }

            // Tree - Overflow collisions
            for (const auto overflowNode : currentTree->m_overflowData)
            {
                if (currentTree->m_data[i].bounds.Intersects(overflowNode.bounds))
                {
                    const std::pair<T, T> pair = std::minmax(
                        currentTree->m_data[i].data, overflowNode.data);

                    if (!collisionPairs.contains(pair))
                    {
                        collisionPairs.insert(pair);
                    }
                }
            }
        }

        // Overflow - Overflow collisions
        for (size_t i = 0; i < currentTree->m_overflowData.size(); ++i)
        {
            for (size_t j = i + 1; j < currentTree->m_overflowData.size(); ++j)
            {
                if (currentTree->m_overflowData[i].bounds.Intersects(
                    currentTree->m_overflowData[j].bounds))
                {
                    const std::pair<T, T> pair =
                        std::minmax(
                            currentTree->m_overflowData[i].data,
                            currentTree->m_overflowData[j].data
                        );

                    if (!collisionPairs.contains(pair))
                    {
                        collisionPairs.insert(pair);
                    }
                }
            }
        }

        if (currentTree->IsSubdivided())
        {
            for (const auto& child : currentTree->m_subTrees)
            {
                stack.emplace(child.get());
            }
        }
    }

    return collisionPairs;
}

template<typename T, size_t CAPACITY>
std::vector<T> QuadTree<T, CAPACITY>::WithinRange(const Math::BoundingBox& box) const
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::WithinRange");

    std::vector<T> result;

    std::stack<const QuadTree<T, CAPACITY>*> stack;
    stack.push(this);

    while (!stack.empty())
    {
        const QuadTree<T, CAPACITY>* currentTree = stack.top();
        stack.pop();

        if (!currentTree->m_boundingBox.Intersects(box))
        {
            continue;
        }

        for (size_t i = 0; i < currentTree->m_dataIndex; i++)
        {
            result.push_back(currentTree->m_data[i].data);
        }

        if (currentTree->IsSubdivided())
        {
            for (const auto& subTrees : currentTree->m_subTrees)
            {
                stack.push(subTrees.get());
            }
        }
    }

    return result;
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::Subdivide()
{
    GenerateSubTrees();

    // Emplace existing data into subtrees
    for (size_t i = 0; i < m_dataIndex; i++)
    {
        for (const auto& quadTree : m_subTrees)
        {
            if (quadTree->m_boundingBox.Intersects(m_data[i].bounds))
            {
                quadTree->AddNode(m_data[i].data, m_data[i].bounds);
            }
        }
    }

    // Emplace overflow
    for (auto overflowData : m_overflowData)
    {
        for (const auto& quadTree : m_subTrees)
        {
            if (quadTree->m_boundingBox.Intersects(overflowData.bounds))
            {
                quadTree->AddNode(overflowData.data, overflowData.bounds);
            }
        }
    }

    m_dataIndex = 0;
    // TODO: Consider shrinking to fit because overflow is cleared.
    m_overflowData.clear();
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::GenerateSubTrees()
{
    // Midpoint
    float midX = (m_boundingBox.start.x + m_boundingBox.end.x) / 2;
    float midY = (m_boundingBox.start.y + m_boundingBox.end.y) / 2;

    Math::BoundingBox nwBox = {m_boundingBox.start, {midX, midY}};
    Math::BoundingBox neBox = {{midX, m_boundingBox.start.y}, {m_boundingBox.end.x, midY}};
    Math::BoundingBox swBox = {{m_boundingBox.start.x, midY}, {midX, m_boundingBox.end.y}};
    Math::BoundingBox seBox = {{midX, midY}, m_boundingBox.end};

    m_subTrees = std::array<std::unique_ptr<QuadTree<T, CAPACITY>>, 4>{
        std::make_unique<QuadTree<T, CAPACITY>>(nwBox),
        std::make_unique<QuadTree<T, CAPACITY>>(neBox),
        std::make_unique<QuadTree<T, CAPACITY>>(swBox),
        std::make_unique<QuadTree<T, CAPACITY>>(seBox)
    };
}

template<typename T, size_t CAPACITY>
QuadTreeDirection QuadTree<T, CAPACITY>::GetChildIndex(const Math::Point& point) const
{
    float midX = (m_boundingBox.start.x + m_boundingBox.end.x) / 2;
    float midY = (m_boundingBox.start.y + m_boundingBox.end.y) / 2;

    if (point.x <= midX)
        return (point.y <= midY) ? NORTH_WEST : SOUTH_WEST;

    return (point.y <= midY) ? NORTH_EAST : SOUTH_EAST;
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::WithinRangeRecursive(
    const Math::BoundingBox& boundingBox, std::vector<T>& result
) const
{
    if (!m_boundingBox.Intersects(boundingBox))
    {
        return;
    }

    for (size_t i = 0; i < m_dataIndex; i++)
    {
        if (boundingBox.Contains(m_data[i].position))
        {
            result.push_back(m_data[i].data);
        }
    }

    // Query Subtrees
    if (IsSubdivided())
    {
        for (const auto& subTree : m_subTrees)
        {
            subTree->WithinRangeRecursive(boundingBox, result);
        }
    }
}

template<typename T, size_t CAPACITY>
bool QuadTree<T, CAPACITY>::IsSubdivided() const
{
    return m_subTrees[0] != nullptr;
}