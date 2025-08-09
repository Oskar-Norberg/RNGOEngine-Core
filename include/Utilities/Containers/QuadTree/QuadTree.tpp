#pragma once

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::Initialize(BoundingBox boundingBox)
{
    m_boundingBox = boundingBox;
    m_dataIndex = 0;
    totalCapacity = 0;

    m_isSubdivided = false;
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::AddNode(T data, Point position)
{
    totalCapacity++;

    if (!CanContain(position))
    {
        return;
    }

    if (m_dataIndex >= CAPACITY && !m_isSubdivided)
    {
        Subdivide();
    }

    if (m_isSubdivided)
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
std::vector<T> QuadTree<T, CAPACITY>::WithinRange(BoundingBox box) const
{
    std::vector<T> result;
    result.reserve(totalCapacity);

    WithinRangeRecursive(box, result);

    return result;
}

template<typename T, size_t CAPACITY>
bool QuadTree<T, CAPACITY>::CanContain(const Point& point) const
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
    m_isSubdivided = true;
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::GenerateSubTrees()
{
    // Midpoint
    float midX = (m_boundingBox.start.x + m_boundingBox.end.x) / 2;
    float midY = (m_boundingBox.start.y + m_boundingBox.end.y) / 2;

    BoundingBox nwBox = {m_boundingBox.start, {midX, midY}};
    BoundingBox neBox = {{midX, m_boundingBox.start.y}, {m_boundingBox.end.x, midY}};
    BoundingBox swBox = {{m_boundingBox.start.x, midY}, {midX, m_boundingBox.end.y}};
    BoundingBox seBox = {{midX, midY}, m_boundingBox.end};

    if (m_subTrees[0] == nullptr)
    {
        m_subTrees = std::array<std::unique_ptr<QuadTree<T, CAPACITY>>, 4>{
            std::make_unique<QuadTree<T, CAPACITY>>(), std::make_unique<QuadTree<T, CAPACITY>>(),
            std::make_unique<QuadTree<T, CAPACITY>>(), std::make_unique<QuadTree<T, CAPACITY>>()
        };
    }

    m_subTrees[NORTH_WEST]->Initialize(nwBox);
    m_subTrees[NORTH_EAST]->Initialize(neBox);
    m_subTrees[SOUTH_WEST]->Initialize(swBox);
    m_subTrees[SOUTH_EAST]->Initialize(seBox);
}

template<typename T, size_t CAPACITY>
QuadTreeDirection QuadTree<T, CAPACITY>::GetChildIndex(const Point& point) const
{
    float midX = (m_boundingBox.start.x + m_boundingBox.end.x) / 2;
    float midY = (m_boundingBox.start.y + m_boundingBox.end.y) / 2;

    if (point.x <= midX)
        return (point.y <= midY) ? NORTH_WEST : SOUTH_WEST;

    return (point.y <= midY) ? NORTH_EAST : SOUTH_EAST;
}

template<typename T, size_t CAPACITY>
void QuadTree<T, CAPACITY>::WithinRangeRecursive(
    BoundingBox boundingBox, std::vector<T>& result
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
    if (m_isSubdivided)
    {
        for (const auto& subTree : m_subTrees)
        {
            subTree->WithinRangeRecursive(boundingBox, result);
        }
    }
}