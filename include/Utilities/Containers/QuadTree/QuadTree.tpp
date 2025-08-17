template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::AddNode(T data, const Math::BoundingBox& bounds)
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::AddNode");

    totalCapacity++;

    auto currentID = ROOT_NODE_ID;
    while (true)
    {
        if (IsFull(currentID) && !IsSubdivided(currentID))
        {
            Subdivide(currentID);
        }

        if (IsSubdivided(currentID))
        {
            bool added = false;

            const std::array<NodeID, 4> children = GetChildren(currentID);
            for (const auto child : children)
            {
                if (GetBoundingBox(child).Contains(bounds))
                {
                    currentID = child;
                    added = true;
                    break;
                }
            }

            if (!added)
            {
                AddDataToNode(data, bounds, currentID);
                break;
            }
        }
        else
        {
            AddDataToNode(data, bounds, currentID);
            break;
        }
    }
}

template<typename T, size_t CAPACITY>
std::vector<std::pair<T, T>> QuadTree<T, CAPACITY>::GetCollisionPairs() const
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::GetCollisionPairs");

    std::vector<std::pair<T, T>> collisionPairs;

    using ParentStack = std::vector<NodeID>;
    std::stack<std::pair<NodeID, ParentStack>> stack;
    stack.push({ROOT_NODE_ID, {}});

    while (!stack.empty())
    {
        auto [currentTree, parents] = std::move(stack.top());
        stack.pop();

        // Tree - Tree
        const auto& currentTreeData = GetNodeData(currentTree);
        const size_t currentTreeSize = currentTreeData.size();

        {
            RNGO_ZONE_SCOPE;
            RNGO_ZONE_NAME_C("QuadTree::Tree - Tree Collision Detection");
            
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
        }

        // Tree - Parent
        {
            RNGO_ZONE_SCOPE;
            RNGO_ZONE_NAME_C("QuadTree::Tree - Parent Collision Detection");
            for (const auto parent : parents)
            {
                const auto& parentData = GetNodeData(parent);
                const size_t parentSize = parentData.size();
                const auto& currentBounds = GetBoundingBox(currentTree);

                for (size_t j = 0; j < parentSize; j++)
                {
                    if (parentData[j].bounds.Intersects(currentBounds))
                    {
                        for (size_t i = 0; i < currentTreeSize; i++)
                        {
                            if (parentData[j].bounds.Intersects(currentTreeData[i].bounds))
                            {
                                collisionPairs.emplace_back(parentData[j].data, currentTreeData[i].data);
                            }
                        }
                    }
                }
            }
        }

        // Enqueue Children
        {
            RNGO_ZONE_SCOPE;
            RNGO_ZONE_NAME_C("QuadTree::Tree - Enqueue Children");
            if (IsSubdivided(currentTree))
            {
                parents.push_back(currentTree);

                auto children = GetChildren(currentTree);
                for (const auto subTree : children)
                {
                    stack.push({subTree, parents});
                }
            }
        }
    }

    return collisionPairs;
}

template<typename T, size_t CAPACITY>
std::array<NodeID, 4> QuadTree<T, CAPACITY>::GetChildren(NodeID id) const
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::GetChildren");
    return m_trees[id].children;
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::Subdivide(NodeID id)
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::Subdivide");

    GenerateSubTrees(id);

    const auto nodeDatas = GetNodeData(id);

    std::vector<QuadTreeData<T>> nodeDataOverflow;

    for (const auto& nodeData : nodeDatas)
    {
        bool added = false;
        std::array<NodeID, 4> children = GetChildren(id);

        for (auto child : children)
        {
            if (GetBoundingBox(child).Contains(nodeData.bounds))
            {
                AddDataToNode(nodeData.data, nodeData.bounds, child);
                added = true;
                break;
            }
        }

        if (!added)
        {
            nodeDataOverflow.push_back(std::move(nodeData));
        }
    }

    SetNodeData(std::move(nodeDataOverflow), id);
}

template<typename T, size_t CAPACITY>
const std::vector<QuadTreeData<T>>& QuadTree<T, CAPACITY>::GetNodeData(NodeID id) const
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::GetNodeData");
    
    return m_trees[id].data;
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::SetNodeData(std::vector<QuadTreeData<T>>&& data, NodeID id)
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::SetNodeData");
    
    m_trees[id].data = std::move(data);
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::AddDataToNode(T data, const Math::BoundingBox& bounds, NodeID id)
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::AddDataToNode");
    
    m_trees[id].data.emplace_back(data, bounds);
}

template<typename T, size_t CAPACITY>
Math::BoundingBox QuadTree<T, CAPACITY>::GetBoundingBox(NodeID id) const
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::GetBoundingBox");
    
    return m_trees[id].bounds;
}

template<typename T, size_t CAPACITY>
bool QuadTree<T, CAPACITY>::IsSubdivided(NodeID id) const
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::IsSubdivided");
    
    return m_trees[id].children[0] != INVALID_NODE_ID;
}

template<typename T, size_t CAPACITY>
bool QuadTree<T, CAPACITY>::IsFull(NodeID id) const
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::IsFull");
    
    return m_trees[id].data.size() >= CAPACITY;
}

template<typename T, size_t CAPACITY>
NodeID QuadTree<T, CAPACITY>::CreateNode(const Math::BoundingBox& bounds)
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::CreateNode");
    
    m_trees.emplace_back(QuadTreeNode<T, CAPACITY>{
        {}, {INVALID_NODE_ID, INVALID_NODE_ID, INVALID_NODE_ID, INVALID_NODE_ID}, bounds});
    return m_trees.size() - 1;
}

template<typename T, size_t CAPACITY>
NodeID QuadTree<T, CAPACITY>::CreateNode(T data, const Math::BoundingBox& bounds)
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::CreateNode");
    
    m_trees.emplace_back(QuadTreeNode<T, CAPACITY>{
        data, {INVALID_NODE_ID, INVALID_NODE_ID, INVALID_NODE_ID, INVALID_NODE_ID}, bounds});
    return m_trees.size() - 1;
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::GenerateSubTrees(NodeID id)
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::GenerateSubTrees");
    
    Math::BoundingBox boundingBox = GetBoundingBox(id);
    // Midpoint
    float midX = (boundingBox.start.x + boundingBox.end.x) / 2;
    float midY = (boundingBox.start.y + boundingBox.end.y) / 2;

    Math::BoundingBox nwBox = {boundingBox.start, {midX, midY}};
    Math::BoundingBox neBox = {{midX, boundingBox.start.y}, {boundingBox.end.x, midY}};
    Math::BoundingBox swBox = {{boundingBox.start.x, midY}, {midX, boundingBox.end.y}};
    Math::BoundingBox seBox = {{midX, midY}, boundingBox.end};

    auto nwChild = CreateNode(nwBox);
    auto neChild = CreateNode(neBox);
    auto swChild = CreateNode(swBox);
    auto seChild = CreateNode(seBox);

    SetChildren(id, {nwChild, neChild, swChild, seChild});
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::SetChildren(NodeID id, std::array<NodeID, 4> children)
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::SetChildren");
    
    m_trees[id].children = children;
}