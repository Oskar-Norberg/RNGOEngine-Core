//
// Created by ringo on 2025-08-16.
//

#include <gtest/gtest.h>

#include <random>
#include <unordered_set>
#include "Utilities/Containers/QuadTree/QuadTree.h"

struct PairHash
{
    template<class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const
    {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

// Line straight up (assumes perfect circle).
float GetRadiusFromBounds(const RNGOEngine::Math::BoundingBox& bounds)
{
    return std::abs(bounds.end.y - bounds.start.y) / 2.0f;
}

TEST(QuadTreeTests, CollisionsNotZero)
{
    constexpr int size = 100;
    constexpr float sizeFloat = static_cast<float>(size);

    RNGOEngine::Containers::Graphs::QuadTree<int, 16> quadTree(
        RNGOEngine::Math::BoundingBox{{0.0f, 0.0f}, {sizeFloat, sizeFloat}});

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            int index = i * size + j;

            RNGOEngine::Math::BoundingBox bounds = {
                {static_cast<float>(i), static_cast<float>(j)},
                {i + 10.0f, j + 10.0f}
            };

            quadTree.AddNode(index, bounds);
        }
    }

    const auto collisionPairs = quadTree.GetCollisionPairs();

    ASSERT_GT(collisionPairs.size(), 0);
}

TEST(QuadTreeTests, NoDuplicateCollisionPairs)
{
    constexpr int size = 40;
    constexpr float sizeFloat = static_cast<float>(size);

    RNGOEngine::Containers::Graphs::QuadTree<int, 16> quadTree(
        RNGOEngine::Math::BoundingBox{{0.0f, 0.0f}, {sizeFloat, sizeFloat}});

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            int index = i * size + j;

            RNGOEngine::Math::BoundingBox bounds = {
                {static_cast<float>(i), static_cast<float>(j)},
                {i + 10.0f, j + 10.0f}
            };

            quadTree.AddNode(index, bounds);
        }
    }

    const auto collisionPairs = quadTree.GetCollisionPairs();

    std::unordered_set<std::pair<int, int>, PairHash> uniquePairs;

    for (auto collisionPair : collisionPairs)
    {
        std::pair<int, int> normalizedPair = {
            std::min(collisionPair.first, collisionPair.second),
            std::max(collisionPair.first, collisionPair.second)
        };

        ASSERT_EQ(uniquePairs.contains(normalizedPair), false);

        uniquePairs.insert(normalizedPair);
    }
}

TEST(QuadTreeTests, NoSelfCollisions)
{
    constexpr int size = 100;
    constexpr float sizeFloat = static_cast<float>(size);

    RNGOEngine::Containers::Graphs::QuadTree<int, 16> quadTree(
        RNGOEngine::Math::BoundingBox{{0.0f, 0.0f}, {sizeFloat, sizeFloat}});

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            int index = i * size + j;

            RNGOEngine::Math::BoundingBox bounds = {
                {static_cast<float>(i), static_cast<float>(j)},
                {i + 10.0f, j + 10.0f}
            };

            quadTree.AddNode(index, bounds);
        }
    }

    const auto collisionPairs = quadTree.GetCollisionPairs();

    std::unordered_set<std::pair<int, int>, PairHash> uniquePairs;

    for (auto collisionPair : collisionPairs)
    {
        ASSERT_EQ(collisionPair.first == collisionPair.second, false);
    }
}

// TODO: This test is quite poorly written.
TEST(QuadTreeTests, MissingCollisions)
{
    struct TestEntity
    {
        int id;
        RNGOEngine::Math::BoundingBox bounds;
    };

    RNGOEngine::Containers::Graphs::QuadTree<TestEntity, 16> quadTree(
        RNGOEngine::Math::BoundingBox{{-150.0f, -150.0f}, {150.0f, 150.0f}});

    std::vector<TestEntity> testEntities;
    size_t randomSeed = 0;
    std::mt19937 randomEngine(randomSeed);
    std::uniform_real_distribution<float> m_randomSizeDist(1.0f, 100.0f);
    std::uniform_real_distribution<float> m_randomPosOffset(-100.0f, 100.0f);

    constexpr size_t nrEntities = 1000;
    for (size_t i = 0; i < nrEntities; ++i)
    {
        TestEntity entity;
        entity.id = static_cast<int>(i);

        float posX = m_randomPosOffset(randomEngine);
        float posY = m_randomPosOffset(randomEngine);
        float size = m_randomSizeDist(randomEngine);

        entity.bounds.start = {posX, posY};
        entity.bounds.end = {posX + size, posY + size};

        testEntities.push_back(entity);
    }

    for (auto testEntity : testEntities)
    {
        quadTree.AddNode(testEntity, testEntity.bounds);
    }

    const auto quadTreeCollisionPairs = quadTree.GetCollisionPairs();
    size_t nrCollisionsQuadTree = 0;
    for (const auto& pair : quadTreeCollisionPairs)
    {
        // get pair x and y
        RNGOEngine::Math::Point pointA = {
            (pair.first.bounds.start.x + pair.first.bounds.end.x) / 2,
            (pair.first.bounds.start.y + pair.first.bounds.end.y) / 2
        };
        RNGOEngine::Math::Point pointB = {
            (pair.second.bounds.start.x + pair.second.bounds.end.x) / 2,
            (pair.second.bounds.start.y + pair.second.bounds.end.y) / 2
        };

        float diffX = pointA.x - pointB.x;
        float diffY = pointA.y - pointB.y;

        float radius = GetRadiusFromBounds(pair.first.bounds);
        radius += GetRadiusFromBounds(pair.second.bounds);

        if (diffX * diffX + diffY * diffY <= radius * radius)
        {
            nrCollisionsQuadTree++;
        }
    }

    // Brute force
    size_t nrCollisionsBruteForce = 0;
    for (int i = 0; i < testEntities.size(); ++i)
    {
        for (int j = i + 1; j < testEntities.size(); ++j)
        {
            RNGOEngine::Math::Point pointA = {
                (testEntities[i].bounds.start.x + testEntities[i].bounds.end.x) / 2,
                (testEntities[i].bounds.start.y + testEntities[i].bounds.end.y) / 2
            };
            RNGOEngine::Math::Point pointB = {
                (testEntities[j].bounds.start.x + testEntities[j].bounds.end.x) / 2,
                (testEntities[j].bounds.start.y + testEntities[j].bounds.end.y) / 2
            };

            float diffX = pointA.x - pointB.x;
            float diffY = pointA.y - pointB.y;

            float radius = GetRadiusFromBounds(testEntities[i].bounds) + GetRadiusFromBounds(
                               testEntities[j].bounds);

            if (diffX * diffX + diffY * diffY <= radius * radius)
            {
                nrCollisionsBruteForce++;
            }
        }
    }

    ASSERT_EQ(nrCollisionsBruteForce, nrCollisionsQuadTree);
}