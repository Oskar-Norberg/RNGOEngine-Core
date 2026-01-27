//
// Created by ringo on 2026-01-27.
//

#pragma once

#include "Point3D.h"

namespace RNGOEngine::Math
{
    template<class T>
    struct AABB3D
    {
        Point3D<T> Start, End;

        bool Contains(Point3D<T> point) const
        {
            // TODO: Use epsilon if T is floating point?
            return point.X >= Start.X && point.X <= End.X && point.Y >= Start.Y && point.Y <= End.Y &&
                   point.Z >= Start.Z && point.Z <= End.Z;
        }

        bool Contains(const AABB3D<T>& other) const
        {
            return Contains(other.Start) && Contains(other.End);
        }

        bool Intersects(const AABB3D<T>& other) const
        {
            return Start.X <= other.End.X && End.X >= other.Start.X && Start.Y <= other.End.Y &&
                   End.Y >= other.Start.Y && Start.Z <= other.End.Z && End.Z >= other.Start.Z;
        }
    };
}