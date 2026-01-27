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
            // TODO: Use epsilon if T is floating point?
            return (point.X >= Start.X && point.X <= End.X) && (point.Y >= Start.Y && point.Y <= End.Y);
        }

        bool Contains(const AABB2D<T>& other) const
        {
            return Contains(other.Start) && Contains(other.End);
        }

        bool Intersects(const AABB2D<T>& other) const
        {
            return !(
                other.End.X < Start.X || other.Start.X > End.X || other.End.Y < Start.Y ||
                other.Start.Y > End.Y
            );
        }
    };
}