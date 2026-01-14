//
// Created by Oskar.Norberg on 2025-12-01.
//

#pragma once

#include <string_view>

namespace RNGOEngine::Data::Shader
{
    struct ShaderDefinition
    {
        std::string_view Name;
        std::string_view Value;
    };
}