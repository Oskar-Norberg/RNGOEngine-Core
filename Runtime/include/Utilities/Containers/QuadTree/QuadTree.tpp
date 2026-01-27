template<typename TData, typename TCoordinate, size_t CAPACITY>
void QuadTree<TData, TCoordinate, CAPACITY>::AddNode(TData data, const Math::AABB2D<TCoordinate>& bounds)
{
    DataID dataID = EmplaceData(data, bounds);
    AddNode(dataID, bounds);
}

template<typename TData, typename TCoordinate, size_t CAPACITY>
void QuadTree<TData, TCoordinate, CAPACITY>::AddNode(TData&& data, const Math::AABB2D<TCoordinate>& bounds)
{
    DataID dataID = EmplaceData(std::forward<TData>(data), bounds);
    
    AddNode(dataID, bounds);
}

template<typename TData, typename TCoordinate, size_t CAPACITY>
std::vector<std::pair<TData, TData>> QuadTree<TData, TCoordinate, CAPACITY>::GetCollisionPairs() const
{
    return GetCollisionPairs(ROOT_NODE_ID);
}

template<typename TData, typename TCoordinate, size_t CAPACITY>
std::vector<std::pair<TData, TData>> QuadTree<TData, TCoordinate, CAPACITY>::GetCollisionPairs(NodeID id) const
{
    RNGO_ZONE_SCOPED_N("QuadTree::GetCollisionPairs");

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

    std::vector<std::pair<TData, TData>> collisionPairs;

    const auto TryAddIntersection = [this, &collisionPairs](
        const DataID nodeAHandle, const DataID nodeBHandle)
    {
        const auto& nodeAData = GetData(nodeAHandle);
        const auto& nodeBData = GetData(nodeBHandle);
        if (nodeAData.bounds.Intersects(nodeBData.bounds))
        {
            collisionPairs.emplace_back(nodeAData.data, nodeBData.data);
        }
    };

    for (const auto currentNodeID : leafNodes)
    {
        const auto& currentTreeDataHandles = GetNodeDataHandles(currentNodeID);
        const size_t currentTreeSize = currentTreeDataHandles.size();

        // Tree - Tree
        {
            RNGO_ZONE_SCOPED_N("QuadTree::Tree - Tree Collision Detection");

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
            RNGO_ZONE_SCOPED_N("QuadTree::Tree - Tree Collision Detection");

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
            RNGO_ZONE_SCOPED_N("QuadTree::Overflow - Overflow Collision Detection");
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

template<typename TData, typename TCoordinate, size_t CAPACITY>
const std::array<NodeID, 4>& QuadTree<TData, TCoordinate, CAPACITY>::GetChildren(NodeID id) const
{
    return m_trees[id].children;
}

template<typename TData, typename TCoordinate, size_t CAPACITY>
bool QuadTree<TData, TCoordinate, CAPACITY>::IsSubdivided(NodeID id) const
{
    return m_trees[id].children[0] != INVALID_NODE_ID;
}

template<typename TData, typename TCoordinate, size_t CAPACITY>
void QuadTree<TData, TCoordinate, CAPACITY>::AddNode(DataID dataID, const Math::AABB2D<TCoordinate>& bounds)
{
    totalCapacity++;

    std::stack<NodeID> stack;
    stack.push(ROOT_NODE_ID);


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

template<typename TData, typename TCoordinate, size_t CAPACITY>
const QuadTreeNode<TCoordinate>& QuadTree<TData, TCoordinate, CAPACITY>::GetNode(NodeID id) const
{
    return m_trees[id];
}

template<typename TData, typename TCoordinate, size_t CAPACITY>
void QuadTree<TData, TCoordinate, CAPACITY>::Subdivide(NodeID id)
{
    GenerateSubTrees(id);
    TransferDataToChildren(id);
}

template<typename TData, typename TCoordinate, size_t CAPACITY>
const std::vector<DataID>& QuadTree<TData, TCoordinate, CAPACITY>::GetNodeDataHandles(NodeID id) const
{
    return m_trees[id].data;
}

template<typename TData, typename TCoordinate, size_t CAPACITY>
const std::vector<DataID>& QuadTree<TData, TCoordinate, CAPACITY>::GetNodeOverflowHandles(NodeID id) const
{
    return m_trees[id].overflow;
}

template<typename TData, typename TCoordinate, size_t CAPACITY>
DataID QuadTree<TData, TCoordinate, CAPACITY>::EmplaceData(TData data, const Math::AABB2D<TCoordinate>& bounds)
{
    m_data.emplace_back(data, bounds);
    return m_data.size() - 1;
}

template<typename TData, typename TCoordinate, size_t CAPACITY>
DataID QuadTree<TData, TCoordinate, CAPACITY>::EmplaceData(TData&& data, const Math::AABB2D<TCoordinate>& bounds)
{
    m_data.emplace_back(std::move(data), bounds);
    return m_data.size() - 1;
}

template<typename TData, typename TCoordinate, size_t CAPACITY>
void QuadTree<TData, TCoordinate, CAPACITY>::AddDataToNode(TData&& data, const Math::AABB2D<TCoordinate>& bounds, NodeID id)
{
    m_data.emplace_back(std::move(data), bounds);

    auto& treeNode = m_trees[id];
    treeNode.data.emplace_back(m_data.size() - 1);
}

template<typename TData, typename TCoordinate, size_t CAPACITY>
void QuadTree<TData, TCoordinate, CAPACITY>::MoveDataToNode(DataID dataID, NodeID nodeID)
{
    auto& node = m_trees[nodeID];

    node.data.emplace_back(dataID);
}

template<typename TData, typename TCoordinate, size_t CAPACITY>
void QuadTree<TData, TCoordinate, CAPACITY>::MoveDataToOverflow(DataID dataID, NodeID nodeID)
{
    m_trees[nodeID].overflow.emplace_back(dataID);
}

template<typename TData, typename TCoordinate, size_t CAPACITY>
void QuadTree<TData, TCoordinate, CAPACITY>::ClearNodeDataHandles(NodeID id)
{
    m_trees[id].data.clear();
    m_trees[id].overflow.clear();
}

template<typename TData, typename TCoordinate, size_t CAPACITY>
const DataEntry<TData, TCoordinate>& QuadTree<TData, TCoordinate, CAPACITY>::GetData(DataID id) const
{
    return m_data[id];
}

template<typename TData, typename TCoordinate, size_t CAPACITY>
bool QuadTree<TData, TCoordinate, CAPACITY>::IsFull(NodeID id) const
{
    return m_trees[id].data.size() >= CAPACITY;
}

template<typename TData, typename TCoordinate, size_t CAPACITY>
NodeID QuadTree<TData, TCoordinate, CAPACITY>::CreateNode(const Math::AABB2D<TCoordinate>& bounds)
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

template<typename TData, typename TCoordinate, size_t CAPACITY>
void QuadTree<TData, TCoordinate, CAPACITY>::GenerateSubTrees(NodeID id)
{
    const auto& bounds = m_trees[id].bounds;
    const Math::AABB2D<TCoordinate>& boundingBox = bounds;
    // Midpoint
    const float midX = (boundingBox.Start.x + boundingBox.End.x) / 2;
    const float midY = (boundingBox.Start.y + boundingBox.End.y) / 2;

    const Math::AABB2D<TCoordinate> nwBox = {boundingBox.Start, {midX, midY}};
    const Math::AABB2D<TCoordinate> neBox = {{midX, boundingBox.Start.y}, {boundingBox.End.x, midY}};
    const Math::AABB2D<TCoordinate> swBox = {{boundingBox.Start.x, midY}, {midX, boundingBox.End.y}};
    const Math::AABB2D<TCoordinate> seBox = {{midX, midY}, boundingBox.End};

    const auto nwChild = CreateNode(nwBox);
    const auto neChild = CreateNode(neBox);
    const auto swChild = CreateNode(swBox);
    const auto seChild = CreateNode(seBox);

    SetChildren(id, {nwChild, neChild, swChild, seChild});
}

template<typename TData, typename TCoordinate, size_t CAPACITY>
void QuadTree<TData, TCoordinate, CAPACITY>::TransferDataToChildren(NodeID id)
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

template<typename TData, typename TCoordinate, size_t CAPACITY>
void QuadTree<TData, TCoordinate, CAPACITY>::SetChildren(NodeID id, std::array<NodeID, 4> children)
{
    m_trees[id].children = children;
}