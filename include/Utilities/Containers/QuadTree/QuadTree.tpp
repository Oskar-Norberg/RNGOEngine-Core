template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::AddNode(T data, const Math::BoundingBox& bounds)
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::AddNode");
    
    totalCapacity++;

    if (m_data.size() >= CAPACITY && !IsSubdivided())
    {
        Subdivide();
    }

    if (IsSubdivided())
    {
        bool added = false;

        for (const auto& subTree : m_subTrees)
        {
            if (subTree->m_boundingBox.Contains(bounds))
            {
                subTree->AddNode(data, bounds);
                added = true;
            }
        }

        if (!added)
        {
            m_data.emplace_back(data, bounds);
        }
    }
    else
    {
        m_data.emplace_back(data, bounds);
    }
}

template<typename T, size_t CAPACITY>
std::vector<std::pair<T, T>> QuadTree<T, CAPACITY>::GetCollisionPairs() const
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::GetCollisionPairs");

    std::vector<std::pair<T, T>> collisionPairs;

    using ParentStack = std::vector<const QuadTree<T, CAPACITY>*>;

    std::stack<std::pair<const QuadTree<T, CAPACITY>*, ParentStack>> stack;
    stack.push({this, {}});

    while (!stack.empty())
    {
        auto [currentTree, parents] = stack.top();
        stack.pop();

        // Tree - Tree
        auto& currentTreeData = currentTree->m_data;
        const size_t currentTreeSize = currentTreeData.size();
        for (size_t i = 0; i < currentTreeSize; i++)
        {
            for (size_t j = i + 1; j < currentTreeSize; j++)
            {
                const auto& nodeA = currentTreeData[i];
                const auto& nodeB = currentTreeData[j];

                if (nodeA.bounds.Intersects(nodeB.bounds))
                {
                    collisionPairs.emplace_back(nodeA.data, nodeB.data);
                }
            }
        }

        // Tree - Parent
        for (const auto* parent : parents)
        {
            auto& parentData = parent->m_data;
            const size_t parentSize = parentData.size();

            for (int i = 0; i < currentTreeSize; ++i)
            {
                for (int j = 0; j < parentSize; ++j)
                {
                    const auto& childNode = currentTreeData[i];
                    const auto& parentNode = parentData[j];

                    if (childNode.bounds.Intersects(parentNode.bounds))
                    {
                        collisionPairs.emplace_back(childNode.data, parentNode.data);
                    }
                }
            }
        }

        // Enqueue Children
        if (currentTree->IsSubdivided())
        {
            parents.push_back(currentTree);
            for (const auto& subTree : currentTree->m_subTrees)
            {
                stack.push({subTree.get(), parents});
            }
        }
    }

    return collisionPairs;
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::Subdivide()
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::Subdivide");
    
    GenerateSubTrees();

    // Emplace existing data into subtrees
    std::vector<QuadTreeNode<T>> m_dataCopy = std::move(m_data);

    size_t dataSize = m_dataCopy.size();
    for (size_t i = 0; i < dataSize; i++)
    {
        bool added = false;

        for (const auto& quadTree : m_subTrees)
        {
            if (quadTree->m_boundingBox.Contains(m_dataCopy[i].bounds))
            {
                // Use the internal method to pass ID
                quadTree->AddNode(m_dataCopy[i].data, m_dataCopy[i].bounds);
                added = true;
                break;
            }
        }

        if (!added)
        {
            m_data.push_back(std::move(m_dataCopy[i]));
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
bool QuadTree<T, CAPACITY>::IsSubdivided() const
{
    return m_subTrees[0] != nullptr;
}