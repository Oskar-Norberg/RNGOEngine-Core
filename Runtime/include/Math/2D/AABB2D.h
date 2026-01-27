//
// Created by Oskar.Norberg on 2025-08-13.
//

#pragma once

#include "Point2D.h"

namespace RNGOEngine::Math
{
    template<class T>
    struct AABB2D
    {
        Point2D<T> Start, End;

        bool Contains(Point2D<T> point) const
        {
            return (point.x >= Start.x && point.x <= End.x) && (point.y >= Start.y && point.y <= End.y);
        }

        bool Contains(const AABB2D<T>& other) const
        {
            return Contains(other.Start) && Contains(other.End);
        }

        bool Intersects(const AABB2D<T>& other) const
        {
            return !(
                other.End.x < Start.x || other.Start.x > End.x || other.End.y < Start.y ||
                other.Start.y > End.y
            );
        }
    };
}