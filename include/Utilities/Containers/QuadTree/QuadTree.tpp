template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::AddNode(T data, const Math::BoundingBox& bounds)
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::AddNode");

    totalCapacity++;

    auto currentID = ROOT_NODE_ID;
    
    while (true)
    {
        bool isSubdivided = IsSubdivided(currentID);
        
        if (IsFull(currentID) && !isSubdivided)
        {
            Subdivide(currentID);
            isSubdivided = true;
        }

        if (isSubdivided)
        {
            bool childCanFit = false;

            const std::array<NodeID, 4>& children = GetChildren(currentID);
            for (const auto child : children)
            {
                if (GetNodeBounds(child).Contains(bounds))
                {
                    currentID = child;
                    childCanFit = true;
                    break;
                }
            }

            if (!childCanFit)
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
        const size_t currentTreeSize = currentTreeDataHandles.size();

        {
            RNGO_ZONE_SCOPE;
            RNGO_ZONE_NAME_C("QuadTree::Tree - Tree Collision Detection");

            for (size_t i = 0; i < currentTreeSize; i++)
            {
                for (size_t j = i + 1; j < currentTreeSize; j++)
                {
                    const auto& nodeAHandle = currentTreeDataHandles[i];
                    const auto& nodeBHandle = currentTreeDataHandles[j];

                    const auto& nodeAData = GetData(nodeAHandle);
                    const auto& nodeBData = GetData(nodeBHandle);

                    if (nodeAData.bounds.Intersects(nodeBData.bounds))
                    {
                        collisionPairs.emplace_back(nodeAData.data, nodeBData.data);
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
                const size_t parentSize = parentDataHandles.size();

                const auto& currentBounds = GetNodeBounds(currentTree);
                const auto& parentBounds = GetNodeBounds(parent);

                if (parentBounds.Intersects(currentBounds))
                {
                    for (size_t j = 0; j < parentSize; j++)
                    {
                        const auto& parentData = GetData(parentDataHandles[j]);
                        for (size_t i = 0; i < currentTreeSize; i++)
                        {
                            const auto& currentTreeData = GetData(currentTreeDataHandles[i]);

                            if (parentData.bounds.Intersects(currentTreeData.bounds))
                            {
                                collisionPairs.emplace_back(parentData.data, currentTreeData.data);
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

    std::vector<DataID> dataHandleOverflows;

    const size_t dataHandlesSize = nodeDataHandles.size();
    for (size_t i = 0; i < dataHandlesSize; i++)
    {
        const auto& currentData = GetData(nodeDataHandles[i]);
        bool added = false;
        const std::array<NodeID, 4>& children = GetChildren(id);

        for (const auto child : children)
        {
            if (GetNodeBounds(child).Contains(currentData.bounds))
            {
                MoveDataToNode(nodeDataHandles[i], child);
                added = true;
                break;
            }
        }

        if (!added)
        {
            dataHandleOverflows.push_back(nodeDataHandles[i]);
        }
    }

    ClearNodeDataHandles(id);

    for (const auto dataHandleOverflow : dataHandleOverflows)
    {
        MoveDataToNode(dataHandleOverflow, id);
    }
}

template<typename T, size_t CAPACITY>
const std::vector<DataID>& QuadTree<T, CAPACITY>::GetNodeDataHandles(NodeID id) const
{
    return m_trees[id].data;
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::AddDataToNode(T data, const Math::BoundingBox& bounds, NodeID id)
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::AddDataToNode");

    m_data.emplace_back(data, bounds);

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
bool QuadTree<T, CAPACITY>::IsSubdivided(NodeID id) const
{
    RNGO_ZONE_SCOPE;
    RNGO_ZONE_NAME_C("QuadTree::IsSubdivided");

    return m_trees[id].children[0] != INVALID_NODE_ID;
}

template<typename T, size_t CAPACITY>
const Math::BoundingBox& QuadTree<T, CAPACITY>::GetNodeBounds(NodeID id) const
{
    return m_treeBounds[m_trees[id].boundsHandle];
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::ClearNodeDataHandles(NodeID id)
{
    m_trees[id].data.clear();
}

template<typename T, size_t CAPACITY>
const DataEntry<T>& QuadTree<T, CAPACITY>::GetData(DataID id) const
{
    return m_data[id];
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

    m_treeBounds.emplace_back(bounds);

    // QuadTreeNode newNode = {
    //     {},
    //     {INVALID_NODE_ID, INVALID_NODE_ID, INVALID_NODE_ID, INVALID_NODE_ID},
    //     m_treeBounds.size() - 1
    // };
    // m_trees.emplace_back(std::move(newNode));

    m_trees.emplace_back(QuadTreeNode{
        {},
        {INVALID_NODE_ID, INVALID_NODE_ID, INVALID_NODE_ID, INVALID_NODE_ID},
        m_treeBounds.size() - 1
    });

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