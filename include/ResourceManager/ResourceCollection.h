//
// Created by Oskar.Norberg on 2025-10-21.
//

#pragma once
#include <vector>

#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

namespace RNGOEngine::Resources
{
    template<typename T>
    struct ResourceCollection
    {
        std::vector<Containers::Vectors::GenerationalKey<T>> Resources;

        // Should I add helper methods here for adding?
        // Meh, feels like bloat. Just access the vector directly for now.
    };
}