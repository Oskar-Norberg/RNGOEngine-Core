template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::AddNode(T&& data, const Math::BoundingBox& bounds)
{
    totalCapacity++;

    std::stack<NodeID> stack;
    stack.push(ROOT_NODE_ID);

    DataID dataID = EmplaceData(std::forward<T>(data), bounds);

    while (!stack.empty())
    {
        const auto currentID = stack.top();
        stack.pop();

        bool isSubdivided = IsSubdivided(currentID);

        if (IsFull(currentID) && !isSubdivided)
        {
            Subdivide(currentID);
            isSubdivided = true;
        }

        if (isSubdivided)
        {
            const std::array<NodeID, 4>& children = GetChildren(currentID);
            for (const auto child : children)
            {
                const auto& childNode = GetNode(child);

                if (childNode.bounds.Contains(bounds))
                {
                    stack.push(child);
                    continue;
                }

                if (childNode.bounds.Intersects(bounds))
                {
                    stack.push(child);
                }
            }
        }
        else
        {
            const auto& currentNode = GetNode(currentID);

            if (currentNode.bounds.Contains(bounds))
            {
                MoveDataToNode(dataID, currentID);
                continue;
            }

            if (currentNode.bounds.Intersects(bounds))
            {
                MoveDataToOverflow(dataID, currentID);
            }
        }
    }
}

template<typename T, size_t CAPACITY>
std::vector<std::pair<T, T>> QuadTree<T, CAPACITY>::GetCollisionPairs() const
{
    GetCollisionPairs(ROOT_NODE_ID);
}

template<typename T, size_t CAPACITY>
std::vector<std::pair<T, T>> QuadTree<T, CAPACITY>::GetCollisionPairs(NodeID id) const
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::GetCollisionPairs");

    std::vector<NodeID> stack;
    stack.emplace_back(id);

    std::vector<NodeID> leafNodes;

    // TODO: Encapsulate this in a function.
    // Get all Leaf Nodes.
    while (!stack.empty())
    {
        const auto currentNodeID = stack.back();
        stack.pop_back();

        if (IsSubdivided(currentNodeID))
        {
            const auto& children = GetChildren(currentNodeID);
            for (const auto child : children)
            {
                stack.emplace_back(child);
            }
        }
        else
        {
            leafNodes.emplace_back(currentNodeID);
        }
    }

    std::unordered_set<uint64_t> seenPairs;
    std::vector<std::pair<T, T>> collisionPairs;

    const auto TryAddIntersection = [this, &seenPairs, &collisionPairs](
        const DataID nodeAHandle, const DataID nodeBHandle)
    {
        const auto& nodeAData = GetData(nodeAHandle);
        const auto& nodeBData = GetData(nodeBHandle);
        if (nodeAData.bounds.Intersects(nodeBData.bounds))
        {
            const auto packedPair = nodeAHandle > nodeBHandle
                                        ? Utilities::Hash::PackUint32Pair(nodeAHandle, nodeBHandle)
                                        : Utilities::Hash::PackUint32Pair(nodeBHandle, nodeAHandle);

            if (seenPairs.insert(packedPair).second)
            {
                collisionPairs.emplace_back(nodeAData.data, nodeBData.data);
            }
        }
    };

    for (const auto currentNodeID : leafNodes)
    {
        const auto& currentTreeDataHandles = GetNodeDataHandles(currentNodeID);
        const size_t currentTreeSize = currentTreeDataHandles.size();

        // Tree - Tree
        {
            RNGO_ZONE_SCOPE;
            RNGO_ZONE_NAME_C("QuadTree::Tree - Tree Collision Detection");

            for (size_t i = 0; i < currentTreeSize; i++)
            {
                for (size_t j = i + 1; j < currentTreeSize; j++)
                {
                    const auto nodeAHandle = currentTreeDataHandles[i];
                    const auto nodeBHandle = currentTreeDataHandles[j];

                    if (nodeAHandle >= nodeBHandle)
                    {
                        continue;
                    }

                    // No risk for duplicates in tree-only collision detection. Skip seenPairs check.
                    const auto& nodeAData = GetData(nodeAHandle);
                    const auto& nodeBData = GetData(nodeBHandle);
                    if (nodeAData.bounds.Intersects(nodeBData.bounds))
                    {
                        collisionPairs.emplace_back(nodeAData.data, nodeBData.data);
                    }
                }
            }
        }

        const auto& currentTreeOverflow = m_trees[currentNodeID].overflow;
        size_t overFlowSize = currentTreeOverflow.size();
        // Tree - Overflow
        {
            RNGO_ZONE_SCOPE;
            RNGO_ZONE_NAME_C("QuadTree::Tree - Tree Collision Detection");

            for (size_t i = 0; i < currentTreeSize; i++)
            {
                for (size_t j = 0; j < overFlowSize; j++)
                {
                    TryAddIntersection(currentTreeDataHandles[i], currentTreeOverflow[j]);
                }
            }
        }

        // Overflow - Overflow
        {
            RNGO_ZONE_SCOPE;
            RNGO_ZONE_NAME_C("QuadTree::Overflow - Overflow Collision Detection");
            for (size_t i = 0; i < overFlowSize; i++)
            {
                for (size_t j = i + 1; j < overFlowSize; j++)
                {
                    TryAddIntersection(currentTreeOverflow[i], currentTreeOverflow[j]);
                }
            }
        }
    }

    return collisionPairs;
}

template<typename T, size_t CAPACITY>
const std::array<NodeID, 4>& QuadTree<T, CAPACITY>::GetChildren(NodeID id) const
{
    return m_trees[id].children;
}

template<typename T, size_t CAPACITY>
bool QuadTree<T, CAPACITY>::IsSubdivided(NodeID id) const
{
    return m_trees[id].children[0] != INVALID_NODE_ID;
}

template<typename T, size_t CAPACITY>
const QuadTreeNode& QuadTree<T, CAPACITY>::GetNode(NodeID id) const
{
    return m_trees[id];
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::Subdivide(NodeID id)
{
    GenerateSubTrees(id);
    TransferDataToChildren(id);
}

template<typename T, size_t CAPACITY>
const std::vector<DataID>& QuadTree<T, CAPACITY>::GetNodeDataHandles(NodeID id) const
{
    return m_trees[id].data;
}

template<typename T, size_t CAPACITY>
const std::vector<DataID>& QuadTree<T, CAPACITY>::GetNodeOverflowHandles(NodeID id) const
{
    return m_trees[id].overflow;
}

template<typename T, size_t CAPACITY>
DataID QuadTree<T, CAPACITY>::EmplaceData(T&& data, const Math::BoundingBox& bounds)
{
    m_data.emplace_back(std::move(data), bounds);
    return m_data.size() - 1;
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::AddDataToNode(T&& data, const Math::BoundingBox& bounds, NodeID id)
{
    m_data.emplace_back(std::move(data), bounds);

    auto& treeNode = m_trees[id];
    treeNode.data.emplace_back(m_data.size() - 1);
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::MoveDataToNode(DataID dataID, NodeID nodeID)
{
    auto& node = m_trees[nodeID];

    node.data.emplace_back(dataID);
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::MoveDataToOverflow(DataID dataID, NodeID nodeID)
{
    m_trees[nodeID].overflow.emplace_back(dataID);
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::ClearNodeDataHandles(NodeID id)
{
    m_trees[id].data.clear();
    m_trees[id].overflow.clear();
}

template<typename T, size_t CAPACITY>
const DataEntry<T>& QuadTree<T, CAPACITY>::GetData(DataID id) const
{
    return m_data[id];
}

template<typename T, size_t CAPACITY>
bool QuadTree<T, CAPACITY>::IsFull(NodeID id) const
{
    return m_trees[id].data.size() >= CAPACITY;
}

template<typename T, size_t CAPACITY>
NodeID QuadTree<T, CAPACITY>::CreateNode(const Math::BoundingBox& bounds)
{
    // QuadTreeNode newNode = {
    //     {},
    //     {INVALID_NODE_ID, INVALID_NODE_ID, INVALID_NODE_ID, INVALID_NODE_ID},
    //     m_treeBounds.size() - 1
    // };
    // m_trees.emplace_back(std::move(newNode));

    m_trees.emplace_back(QuadTreeNode{
        {},
        {},
        {INVALID_NODE_ID, INVALID_NODE_ID, INVALID_NODE_ID, INVALID_NODE_ID},
        bounds
    });

    return m_trees.size() - 1;
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::GenerateSubTrees(NodeID id)
{
    const auto& bounds = m_trees[id].bounds;
    const Math::BoundingBox& boundingBox = bounds;
    // Midpoint
    const float midX = (boundingBox.start.x + boundingBox.end.x) / 2;
    const float midY = (boundingBox.start.y + boundingBox.end.y) / 2;

    const Math::BoundingBox nwBox = {boundingBox.start, {midX, midY}};
    const Math::BoundingBox neBox = {{midX, boundingBox.start.y}, {boundingBox.end.x, midY}};
    const Math::BoundingBox swBox = {{boundingBox.start.x, midY}, {midX, boundingBox.end.y}};
    const Math::BoundingBox seBox = {{midX, midY}, boundingBox.end};

    const auto nwChild = CreateNode(nwBox);
    const auto neChild = CreateNode(neBox);
    const auto swChild = CreateNode(swBox);
    const auto seChild = CreateNode(seBox);

    SetChildren(id, {nwChild, neChild, swChild, seChild});
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::TransferDataToChildren(NodeID id)
{
    const auto& nodeDataHandles = GetNodeDataHandles(id);

    size_t dataHandlesSize = nodeDataHandles.size();
    for (size_t i = 0; i < dataHandlesSize; i++)
    {
        const auto& currentData = GetData(nodeDataHandles[i]);
        const std::array<NodeID, 4>& children = GetChildren(id);

        for (const auto child : children)
        {
            const auto& childNode = GetNode(child);

            if (childNode.bounds.Contains(currentData.bounds))
            {
                MoveDataToNode(nodeDataHandles[i], child);
                continue;
            }

            if (childNode.bounds.Intersects(currentData.bounds))
            {
                MoveDataToOverflow(nodeDataHandles[i], child);
            }
        }
    }

    const auto& overflowHandles = GetNodeOverflowHandles(id);
    for (const auto dataHandle : overflowHandles)
    {
        const auto& data = GetData(dataHandle);
        const std::array<NodeID, 4>& children = GetChildren(id);
        for (const auto child : children)
        {
            const auto& childNode = GetNode(child);

            if (childNode.bounds.Intersects(data.bounds))
            {
                MoveDataToOverflow(dataHandle, child);
            }
        }
    }

    ClearNodeDataHandles(id);
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::SetChildren(NodeID id, std::array<NodeID, 4> children)
{
    m_trees[id].children = children;
}