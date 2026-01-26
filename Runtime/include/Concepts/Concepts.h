//
// Created by SkummisDenAndre on 2025-06-21.
//

#pragma once

#include <concepts>

namespace RNGOEngine::Concepts
{
    template<class T, class U>
    concept DerivedFrom = std::derived_from<T, U>;

    template<class T, class U >
    concept ConstructibleFrom = std::constructible_from<T, U>;

    template<class T>
    concept DefaultConstructible = std::default_initializable<T>;
}