//
// Created by Oskar.Norberg on 2025-12-01.
//

#pragma once
#include "ShaderDefinition.h"

namespace RNGOEngine::Data::Shader
{
    constexpr auto INCLUDE_DIRECTIVE = "#include";

    // Attribute Pointers
    constexpr ShaderDefinition POSITION_ATTRIBUTE_POINTER = {"POSITION_ATTRIBUTE_POINTER", 0};
    constexpr ShaderDefinition NORMAL_ATTRIBUTE_POINTER = {"NORMAL_ATTRIBUTE_POINTER", 1};
    constexpr ShaderDefinition TEXCOORD_ATTRIBUTE_POINTER = {"TEXCOORD_ATTRIBUTE_POINTER", 2};

    // Lighting
    constexpr ShaderDefinition NR_OF_POINTLIGHTS = {"NR_OF_POINTLIGHTS", 16};
    constexpr ShaderDefinition NR_OF_SPOTLIGHTS = {"NR_OF_SPOTLIGHTS", 16};

    constexpr ShaderDefinition RNGO_SHADER_DEFINITIONS[] = {
        POSITION_ATTRIBUTE_POINTER, NORMAL_ATTRIBUTE_POINTER, TEXCOORD_ATTRIBUTE_POINTER, NR_OF_POINTLIGHTS,
        NR_OF_SPOTLIGHTS
    };
}