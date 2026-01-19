//
// Created by Oskar.Norberg on 2025-12-01.
//

#pragma once

#include "ShaderDefinition.h"

namespace RNGOEngine::Data::Shader
{
    constexpr auto INCLUDE_DIRECTIVE = "#include";
    constexpr auto INCLUDE_START = '<';
    constexpr auto INCLUDE_END = '>';

    constexpr auto VERTEX_SHADER_START = "#vertex_shader";
    constexpr auto FRAGMENT_SHADER_START = "#fragment_shader";

    // Phong Material
    constexpr int ALBEDO_TEXTURE_SLOT = 0;
    constexpr ShaderDefinition ALBEDO_TEXTURE = {"ALBEDO_TEXTURE", "ALBEDO_SAMPLER"};
    constexpr int NORMAL_TEXTURE_SLOT = 1;
    constexpr ShaderDefinition NORMAL_TEXTURE = {"NORMAL_TEXTURE", "NORMAL_SAMPLER"};
    constexpr int SPECULAR_TEXTURE_SLOT = 2;
    constexpr ShaderDefinition SPECULAR_TEXTURE = {"SPECULAR_TEXTURE", "SPECULAR_SAMPLER"};

    // Shadow Mapping
    constexpr int DIRECTIONAL_SHADOWMAP_TEXTURE_SLOT = 3;
    constexpr ShaderDefinition DIRECTIONAL_SHADOWMAP_TEXTURE = {
        "DIRECTIONAL_SHADOWMAP_TEXTURE", "DIRECTIONAL_SHADOWMAP_SAMPLER"
    };
    constexpr ShaderDefinition MIN_SHADOW_BIAS = {
        "MIN_SHADOW_BIAS", "0.05"
    };
    constexpr ShaderDefinition MAX_SHADOW_BIAS = {
        "MAX_SHADOW_BIAS", "0.05"
    };

    // Attribute Pointers
    // TODO: Stupid duplicates
    constexpr int POSITION_ATTRIBUTE_POINTER_INDEX = 0;
    constexpr ShaderDefinition POSITION_ATTRIBUTE_POINTER = {"POSITION_ATTRIBUTE_POINTER", "0"};
    constexpr int NORMAL_ATTRIBUTE_POINTER_INDEX = 1;
    constexpr ShaderDefinition NORMAL_ATTRIBUTE_POINTER = {"NORMAL_ATTRIBUTE_POINTER", "1"};
    constexpr int TEXCOORD_ATTRIBUTE_POINTER_INDEX = 2;
    constexpr ShaderDefinition TEXCOORD_ATTRIBUTE_POINTER = {"TEXCOORD_ATTRIBUTE_POINTER", "2"};

    // Lighting
    constexpr int NR_OF_POINTLIGHTS = 16;
    constexpr ShaderDefinition NR_OF_POINTLIGHTS_DEF = {"NR_OF_POINTLIGHTS", "16"};
    constexpr int NR_OF_SPOTLIGHTS = 8;
    constexpr ShaderDefinition NR_OF_SPOTLIGHTS_DEF = {"NR_OF_SPOTLIGHTS", "8"};

    constexpr ShaderDefinition RNGO_SHADER_DEFINITIONS[] = {
        ALBEDO_TEXTURE,
        NORMAL_TEXTURE,
        SPECULAR_TEXTURE,

        DIRECTIONAL_SHADOWMAP_TEXTURE,
        MIN_SHADOW_BIAS,
        MAX_SHADOW_BIAS,

        POSITION_ATTRIBUTE_POINTER,
        NORMAL_ATTRIBUTE_POINTER,
        TEXCOORD_ATTRIBUTE_POINTER,

        NR_OF_POINTLIGHTS_DEF,
        NR_OF_SPOTLIGHTS_DEF
    };
}