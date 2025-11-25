//
// Created by Oskar.Norberg on 2025-09-11.
//

#pragma once

#include <expected>

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "Assets/AssetLoaders/ShaderLoader.h"
#include "Renderer/RenderID.h"
#include "Utilities/AssetCache/AssetCache.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

namespace RNGOEngine
{
    namespace Resources
    {
        class ResourceManager;
    }
}

namespace RNGOEngine::AssetHandling
{
    using ShaderManagerID = unsigned int;

    enum class ShaderManagerError
    {
        None,
        // TODO:
    };

    struct RuntimeShaderProgramData
    {
        Containers::GenerationalKey<Core::Renderer::ShaderProgramID> ProgramKey;
    };

    // TODO: Separate Shader and ShaderProgram into separate managers.
    class ShaderManager
    {
    public:
        explicit ShaderManager(Resources::ResourceManager& resourceManager);

    public:
        std::expected<ShaderAsset, ShaderManagerError> UploadShader(
            const AssetHandle& assetHandle, std::string_view shaderSource, Core::Renderer::ShaderType type
        );
        void DestroyShader(const AssetHandle& assetHandle);

    public:
        Containers::GenerationalKey<RuntimeShaderProgramData> CreateShaderProgram(
            const AssetHandle& vertexShader, const AssetHandle& fragmentShader
        );

    public:
        Core::Renderer::ShaderProgramID GetShaderProgram(
            const Containers::GenerationalKey<RuntimeShaderProgramData>& key
        );

    private:
        Resources::ResourceManager& m_resourceManager;

    private:
        std::unordered_map<AssetHandle, std::shared_ptr<ShaderAsset>> m_handleToShader;

    private:
        Containers::GenerationalVector<RuntimeShaderProgramData> m_shaderPrograms;
        std::unordered_map<
            std::pair<AssetHandle, AssetHandle>, Containers::GenerationalKey<RuntimeShaderProgramData>,
            Utilities::Hash::PairHash>
            m_shaderProgramCache;
    };
}