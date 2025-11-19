//
// Created by Oskar.Norberg on 2025-08-27.
//

#pragma once

#include <glm/glm.hpp>
#include <string_view>

#include "Assets/Asset.h"

namespace RNGOEngine
{
    namespace AssetHandling
    {
        class MaterialManager;
    }
}

// TODO: Renderer/Handles makes no sense for something as high level as a MaterialHandle.
namespace RNGOEngine::Core::Renderer
{
    class MaterialHandle
    {
    public:
        explicit MaterialHandle(
            const AssetHandling::AssetHandle& materialKey, AssetHandling::MaterialManager& manager
        )
            : m_materialKey(materialKey), m_manager(manager)
        {
        }

        void SetTexture(const AssetHandling::AssetHandle& texture, const int slot) const;
        void SetBool(const std::string_view name, const bool value) const;
        void SetInt(const std::string_view name, const int value) const;
        void SetFloat(const std::string_view name, const float value) const;
        void SetVec2(const std::string_view name, const glm::vec2& value) const;
        void SetVec3(const std::string_view name, const glm::vec3& value) const;
        void SetVec4(const std::string_view name, const glm::vec4& value) const;
        void SetMat4(const std::string_view name, const glm::mat4& value) const;

        AssetHandling::AssetHandle GetMaterialAssetHandle() const
        {
            return m_materialKey;
        }

    private:
        AssetHandling::AssetHandle m_materialKey;
        AssetHandling::MaterialManager& m_manager;
    };
}