//
// Created by Oskar.Norberg on 2025-09-11.
//

#pragma once

#include <expected>

#include "AssetManager/AssetLoaders/ShaderLoader.h"
#include "Renderer/RenderID.h"
#include "Utilities/AssetCache/AssetCache.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"
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
    using ShaderManagerID = unsigned int;
    
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

        std::expected<ShaderManagerID, ShaderManagerError> LoadShaderProgram(
            const std::filesystem::path& vertexShaderPath,
            const std::filesystem::path& fragmentShaderPath);

        std::optional<Core::Renderer::ShaderProgramID> GetShaderProgram(ShaderManagerID id) const;

    private:
        ShaderLoader m_shaderLoader;
        Resources::ResourceManager& m_resourceManager;

    private:
        std::vector<Containers::Vectors::GenerationalKey<Core::Renderer::ShaderID>> m_shaders;
        std::vector<Containers::Vectors::GenerationalKey<Core::Renderer::ShaderProgramID>> m_shaderPrograms;

    private:
        // Links a file path to an index in m_shaders
        Utilities::AssetCache<std::filesystem::path, ShaderManagerID> m_shaderCache;

        // Links two indices in m_shaders to an index in m_shaderPrograms
        Utilities::AssetCache<std::pair<ShaderManagerID, ShaderManagerID>, Core::Renderer::ShaderProgramID, Utilities::Hash::PairHash>
        m_shaderProgramCache;

    private:
        std::expected<ShaderManagerID, ShaderManagerError> CreateShader(
            const std::filesystem::path& path, Core::Renderer::ShaderType type);

        std::expected<ShaderManagerID, ShaderManagerError> CreateShaderProgram(
            Core::Renderer::ShaderID vertexShader,
            Core::Renderer::ShaderID fragmentShader);
    };
}