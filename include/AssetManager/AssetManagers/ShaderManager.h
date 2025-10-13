//
// Created by Oskar.Norberg on 2025-09-11.
//

#pragma once

#include <expected>

#include "AssetManager/AssetLoaders/ShaderLoader.h"
#include "Renderer/RenderID.h"
#include "Utilities/AssetCache/AssetCache.h"
#include "Utilities/Hash/PairHash.h"

namespace RNGOEngine
{
    namespace Resources
    {
        class ResourceManager;
    }
}

namespace RNGOEngine::AssetHandling
{
    enum class ShaderManagerError
    {
        None,
        // Resources
        CompilationFailed,
        LinkingFailed,
        // Preprocessing
        FileNotFound,
        MalformedInclude,
        TokenNotFound,
    };

    class ShaderManager
    {
    public:
        explicit ShaderManager(Resources::ResourceManager& resourceManager,
                               const AssetFileFetcher& assetFetcher);

        std::expected<Core::Renderer::ShaderProgramID, ShaderManagerError> LoadShaderProgram(
            const std::filesystem::path& vertexShaderPath,
            const std::filesystem::path& fragmentShaderPath);

    private:
        ShaderLoader m_shaderLoader;
        Resources::ResourceManager& m_resourceManager;

    private:
        Utilities::AssetCache<std::filesystem::path, Core::Renderer::ShaderID> m_shaderCache;
        Utilities::AssetCache<std::pair<Core::Renderer::ShaderID, Core::Renderer::ShaderID>,
                              Core::Renderer::ShaderID, Utilities::Hash::PairHash> m_shaderProgramCache;

    private:
        std::expected<Core::Renderer::ShaderID, ShaderManagerError> CreateShader(
            const std::filesystem::path& path, Core::Renderer::ShaderType type);

        std::expected<Core::Renderer::ShaderProgramID, ShaderManagerError> CreateShaderProgram(
            Core::Renderer::ShaderID vertexShader,
            Core::Renderer::ShaderID fragmentShader);
    };
}