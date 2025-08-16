template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::AddNode(T data, const Math::BoundingBox& boundingBox)
{
    totalCapacity++;

    if (!m_boundingBox.Contains(boundingBox))
    {
        return;
    }

    if (m_data.size() >= CAPACITY && !IsSubdivided())
    {
        Subdivide();
    }

    bool couldAdd = false;

    if (IsSubdivided())
    {
        for (const auto& subTree : m_subTrees)
        {
            if (subTree->m_boundingBox.Contains(boundingBox))
            {
                couldAdd = true;
                subTree->AddNode(data, boundingBox);
                break;
            }
        }
    }

    if (!couldAdd)
    {
        m_data.emplace_back(data, boundingBox);
    }
}

template<typename T, size_t CAPACITY>
std::vector<std::pair<T, T>> QuadTree<T, CAPACITY>::GetCollisionPairs() const
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::GetCollisionPairs");

    std::vector<std::pair<T, T>> collisionPairs;

    std::stack<const QuadTree*> stack;
    stack.emplace(this);

    while (!stack.empty())
    {
        const QuadTree* currentTree = std::move(stack.top());
        stack.pop();

        size_t currentTreeDataSize = currentTree->m_data.size();
        for (size_t i = 0; i < currentTreeDataSize; ++i)
        {
            for (size_t j = i + 1; j < currentTreeDataSize; ++j)
            {
                if (currentTree->m_data[i].bounds.Intersects(currentTree->m_data[j].bounds))
                {
                    collisionPairs.emplace_back(
                    currentTree->m_data[i].data, currentTree->m_data[j].data);
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
std::vector<T> QuadTree<T, CAPACITY>::WithinRange(Math::BoundingBox box) const
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
    
        for (size_t i = 0; i < currentTree->m_data.size(); i++)
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

    std::vector<QuadTreeNode<T>> m_dataCopy = std::move(m_data);

    // Emplace existing data into subtrees
    const size_t dataSize = m_dataCopy.size();
    for (size_t i = 0; i < dataSize; i++)
    {
        bool moved = false;
        
        for (const auto& quadTree : m_subTrees)
        {
            if (quadTree->m_boundingBox.Contains(m_dataCopy[i].bounds))
            {
                quadTree->AddNode(m_dataCopy[i].data, m_dataCopy[i].bounds);
                moved = true;
                break;
            }
        }

        if (!moved)
        {
            m_data.emplace_back(m_dataCopy[i].data, m_dataCopy[i].bounds);
        }
    }
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
    Math::BoundingBox boundingBox, std::vector<T>& result
) const
{
    if (!m_boundingBox.Intersects(boundingBox))
    {
        return;
    }

    const size_t dataSize = m_data.size();
    for (size_t i = 0; i < dataSize; i++)
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