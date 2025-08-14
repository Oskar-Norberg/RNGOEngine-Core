template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::AddNode(T data, Math::Point position)
{
    totalCapacity++;

    if (!CanContain(position))
    {
        return;
    }

    if (m_dataIndex >= CAPACITY && !IsSubdivided())
    {
        Subdivide();
    }

    if (IsSubdivided())
    {
        const auto childIndex = GetChildIndex(position);
        m_subTrees[childIndex]->AddNode(data, position);
    }
    else
    {
        m_data[m_dataIndex].data = data;
        m_data[m_dataIndex].position = position;
        m_dataIndex++;
    }
}

template<typename T, size_t CAPACITY>
std::vector<std::pair<T, T>> QuadTree<T, CAPACITY>::GetCollisionPairs() const
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::GetCollisionPairs");

    std::vector<std::pair<T, T>> collisionPairs;
    collisionPairs.reserve(EstimatedNrOfCollisionPairs());

    struct TreeAncestry
    {
        const QuadTree* node;
        std::vector<const QuadTreeNode<T>*> parentData;
    };

    std::stack<TreeAncestry> stack;
    stack.emplace(this, std::vector<const QuadTreeNode<T>*>());

    while (!stack.empty())
    {
        TreeAncestry currentTree = std::move(stack.top());
        stack.pop();

        const auto* node = currentTree.node;

        for (size_t i = 0; i < node->m_dataIndex; ++i)
        {
            for (size_t j = i + 1; j < node->m_dataIndex; ++j)
            {
                collisionPairs.emplace_back(
                    node->m_data[i].data, node->m_data[j].data);
            }
        }

        for (size_t i = 0; i < node->m_dataIndex; ++i)
        {
            for (const auto* parentNode : currentTree.parentData)
            {
                collisionPairs.emplace_back(
                    node->m_data[i].data, parentNode->data);
            }
        }

        if (node->IsSubdivided())
        {
            std::vector<const QuadTreeNode<T>*> newParentData = currentTree.parentData;

            for (size_t i = 0; i < node->m_dataIndex; ++i)
            {
                newParentData.emplace_back(&node->m_data[i]);
            }

            for (const auto& child : node->m_subTrees)
            {
                stack.emplace(child.get(), newParentData);
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
    result.reserve(totalCapacity);

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
bool QuadTree<T, CAPACITY>::CanContain(const Math::Point& point) const
{
    return m_boundingBox.Contains(point);
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
            if (quadTree->CanContain(m_data[i].position))
            {
                quadTree->AddNode(m_data[i].data, m_data[i].position);
                break;
            }
        }
    }

    m_dataIndex = 0;
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