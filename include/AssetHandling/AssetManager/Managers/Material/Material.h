//
// Created by Oskar.Norberg on 2025-10-20.
//

#pragma once

#include <string>
#include <variant>
#include <vector>
#include <glm/glm.hpp>

#include "AssetHandling/AssetDatabase/AssetHandle.h"
#include "AssetHandling/AssetManager/Managers/ShaderManager.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

namespace RNGOEngine::AssetHandling
{
    struct MaterialTextureSpecification
    {
        AssetHandle textureHandle;
        int slot;
    };

    struct MaterialParameter
    {
        std::string name;
        std::variant<bool, int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat4,
             MaterialTextureSpecification> data;
    };

    struct MaterialSpecification
    {
        Containers::Vectors::GenerationalKey<ShaderManagerProgramData> shader;

        // Consider stack allocated array with max size?
        std::vector<MaterialParameter> uniforms;
    };
}