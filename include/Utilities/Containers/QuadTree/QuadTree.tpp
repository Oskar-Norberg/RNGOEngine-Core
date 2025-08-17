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
                if (GetNodeBounds(child).Contains(bounds))
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
        const auto& currentTreeDataHandles = GetNodeDataHandles(currentTree);
        const size_t currentTreeSize = currentTreeDataHandles.second;

        {
            RNGO_ZONE_SCOPE;
            RNGO_ZONE_NAME_C("QuadTree::Tree - Tree Collision Detection");

            for (size_t i = 0; i < currentTreeSize; i++)
            {
                for (size_t j = i + 1; j < currentTreeSize; j++)
                {
                    const auto& nodeAHandle = currentTreeDataHandles.first[i];
                    const auto& nodeBHandle = currentTreeDataHandles.first[j];

                    const auto& nodeABounds = GetDataBounds(nodeAHandle);
                    const auto& nodeBBounds = GetDataBounds(nodeBHandle);

                    if (nodeABounds.Intersects(nodeBBounds))
                    {
                        collisionPairs.emplace_back(GetData(nodeAHandle), GetData(nodeBHandle));
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
                const auto& parentDataHandles = GetNodeDataHandles(parent);
                const size_t parentSize = parentDataHandles.second;
                
                const auto& currentBounds = GetNodeBounds(currentTree);
                const auto& parentBounds = GetNodeBounds(parent);

                if (parentBounds.Intersects(currentBounds))
                {
                    for (size_t j = 0; j < parentSize; j++)
                    {
                        const auto& parentData = GetData(parentDataHandles.first[j]);
                        const auto& parentDataBounds = GetDataBounds(parentDataHandles.first[j]);
                        for (size_t i = 0; i < currentTreeSize; i++)
                        {
                            const auto& currentTreeData = GetData(currentTreeDataHandles.first[i]);
                            const auto& currentTreeDataBounds = GetDataBounds(
                                currentTreeDataHandles.first[i]);

                            if (parentDataBounds.Intersects(currentTreeDataBounds))
                            {
                                collisionPairs.emplace_back(parentData, currentTreeData);
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
const std::array<NodeID, 4>& QuadTree<T, CAPACITY>::GetChildren(NodeID id) const
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

    const auto nodeDataHandles = GetNodeDataHandles(id);

    std::vector<DataHandle> dataHandleOverflows;

    for (size_t i = 0; i < nodeDataHandles.second; i++)
    {
        const auto& currentDataBounds = GetDataBounds(nodeDataHandles.first[i]);
        bool added = false;
        const std::array<NodeID, 4>& children = GetChildren(id);

        for (const auto child : children)
        {
            if (GetNodeBounds(child).Contains(currentDataBounds))
            {
                MoveDataToNode(nodeDataHandles.first[i], child);
                added = true;
                break;
            }
        }

        if (!added)
        {
            dataHandleOverflows.push_back(nodeDataHandles.first[i]);
        }
    }

    ClearNodeDataHandles(id);

    for (const auto dataHandleOverflow : dataHandleOverflows)
    {
        MoveDataToNode(dataHandleOverflow, id);
    }
}

template<typename T, size_t CAPACITY>
std::pair<const std::array<DataHandle, CAPACITY>&, size_t> QuadTree<
    T, CAPACITY>::GetNodeDataHandles(NodeID id) const
{
    return std::make_pair(m_trees[id].data, m_trees[id].dataIndex);
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::AddDataToNode(T data, const Math::BoundingBox& bounds, NodeID id)
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::AddDataToNode");

    m_data.emplace_back(data);
    m_dataBounds.emplace_back(bounds);

    auto& treeNode = m_trees[id];
    treeNode.data[treeNode.dataIndex++] = {m_data.size() - 1, m_dataBounds.size() - 1};
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::MoveDataToNode(DataHandle dataHandle, NodeID nodeID)
{
    auto& node = m_trees[nodeID];
    node.data[node.dataIndex++] = dataHandle;
}

template<typename T, size_t CAPACITY>
bool QuadTree<T, CAPACITY>::IsSubdivided(NodeID id) const
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::IsSubdivided");

    return m_trees[id].children[0] != INVALID_NODE_ID;
}

template<typename T, size_t CAPACITY>
const Math::BoundingBox& QuadTree<T, CAPACITY>::GetNodeBounds(NodeID id) const
{
    return m_treeBounds[id];
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::ClearNodeDataHandles(NodeID id)
{
    m_trees[id].dataIndex = 0;
}

template<typename T, size_t CAPACITY>
const T& QuadTree<T, CAPACITY>::GetData(DataHandle id) const
{
    return m_data[id.dataID];
}

template<typename T, size_t CAPACITY>
const Math::BoundingBox& QuadTree<T, CAPACITY>::GetDataBounds(DataHandle id) const
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::GetBoundingBox");

    return m_dataBounds[id.boundsID];
}

template<typename T, size_t CAPACITY>
bool QuadTree<T, CAPACITY>::IsFull(NodeID id) const
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::IsFull");

    return m_trees[id].dataIndex >= CAPACITY;
}

template<typename T, size_t CAPACITY>
NodeID QuadTree<T, CAPACITY>::CreateNode(const Math::BoundingBox& bounds)
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::CreateNode");

    m_treeBounds.emplace_back(bounds);

    m_trees.emplace_back(QuadTreeNode<T, CAPACITY>{
            {}, 0, {INVALID_NODE_ID, INVALID_NODE_ID, INVALID_NODE_ID, INVALID_NODE_ID}, m_treeBounds.size() - 1}
    );
    
    return m_trees.size() - 1;
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::GenerateSubTrees(NodeID id)
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::GenerateSubTrees");

    Math::BoundingBox boundingBox = GetNodeBounds(id);
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