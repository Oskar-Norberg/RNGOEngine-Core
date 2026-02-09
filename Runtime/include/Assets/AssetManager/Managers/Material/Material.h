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
    struct MaterialTextureSpecification
    {
        AssetHandle TextureHandle;
        int Slot;
    };

    using MaterialParameter = std::variant<
        bool, int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat4, MaterialTextureSpecification>;

    struct MaterialParametersContainer
    {
        std::string Name{};
        MaterialParameter Parameter{};
    };

    class MaterialParameters
    {
    public:
        void Add(std::string_view name, const MaterialParameter& parameter, const bool overwrite = false)
        {
            const auto it = std::ranges::find_if(
                Parameters,
                [&](const auto& paramPair)
                {
                    return paramPair.Name == name;
                }
            );

            if (it != Parameters.end())
            {
                if (overwrite)
                {
                    it->Parameter = parameter;
                }
            }
            else
            {
                Parameters.emplace_back(std::string(name), parameter);
            }
        }

        const std::vector<MaterialParametersContainer>& GetAll() const
        {
            return Parameters;
        }

    private:
        // Consider stack allocated array with max size?
        std::vector<MaterialParametersContainer> Parameters{};
    };

    struct MaterialSpecification
    {
        ShaderHandle ShaderHandle{};

        MaterialParameters Parameters{};
    };
}