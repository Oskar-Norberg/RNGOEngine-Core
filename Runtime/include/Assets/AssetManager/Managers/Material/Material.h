//
// Created by Oskar.Norberg on 2025-10-20.
//

#pragma once

#include <glm/glm.hpp>
#include <string>
#include <variant>
#include <vector>

#include "Assets/Asset.h"
#include "Renderer/DrawQueue.h"

namespace RNGOEngine::AssetHandling
{
    // TODO: PascalCase case public members
    struct MaterialTextureSpecification
    {
        AssetHandle TextureHandle;
        int Slot;
    };

    struct MaterialParameter
    {
        std::string Name;
        std::variant<
            bool, int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat4, MaterialTextureSpecification>
            Value;
    };

    struct MaterialParameters
    {
        // Consider stack allocated array with max size?
        std::vector<MaterialParameter> Parameters{};
    };
}