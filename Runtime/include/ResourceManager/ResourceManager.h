//
// Created by ringo on 2025-10-05.
//

#pragma once

#include <functional>
#include <optional>
#include <string_view>

#include "Data/MeshData.h"
#include "FrameTargetManager/RenderTargetManager.h"
#include "MeshResourceManager/MeshResourceManager.h"
#include "Renderer/RenderID.h"
#include "ResourceCollection.h"
#include "ResourceTracker.h"
#include "ShaderResourceManager/ShaderResourceManager.h"
#include "TextureResourceManager/TextureResourceManager.h"
#include "Utilities/Singleton/Singleton.h"

namespace RNGOEngine
{
    namespace Core
    {
        namespace Renderer
        {
            class IRenderer;
        }
    }
}

// TODO: should probably be in Core namespace. Maybe even RNGOEngine::Core::Renderer::Resources?
// TODO: This should really work something like a service locator, instead of being a facade for all resource managers.
namespace RNGOEngine::Resources
{
    class ResourceManager : public Utilities::Singleton<ResourceManager>
    {
    public:
        explicit ResourceManager(RNGOEngine::Core::Renderer::IRenderer& renderer);

        // Non-const Getters
    public:
        MeshResourceManager& GetMeshResourceManager()
        {
            return m_meshResourceManager;
        }

        ShaderResourceManager& GetShaderResourceManager()
        {
            return m_shaderResourceManager;
        }

        TextureResourceManager& GetTextureResourceManager()
        {
            return m_textureResourceManager;
        }

        RenderTargetManager& GetRenderTargetManager()
        {
            return m_renderTargetManager;
        }

        // Const Getters
    public:
        const MeshResourceManager& GetMeshResourceManager() const
        {
            return m_meshResourceManager;
        }

        const ShaderResourceManager& GetShaderResourceManager() const
        {
            return m_shaderResourceManager;
        }

        const TextureResourceManager& GetTextureResourceManager() const
        {
            return m_textureResourceManager;
        }

        const RenderTargetManager& GetRenderTargetManager() const
        {
            return m_renderTargetManager;
        }

    private:
        MeshResourceManager m_meshResourceManager;
        ShaderResourceManager m_shaderResourceManager;
        TextureResourceManager m_textureResourceManager;
        RenderTargetManager m_renderTargetManager;
    };
}