//
// Created by SkummisDenAndre on 2025-06-21.
//

#pragma once

#include <concepts>

template<class T, class U>
concept DerivedFrom = std::derived_from<T, U>;
