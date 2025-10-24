//
// Created by Oskar.Norberg on 2025-09-11.
//

#pragma once

#include <expected>

#include "AssetHandling/AssetLoaders/ShaderLoader.h"
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
        ShaderNotFound,
        CompilationFailed,
        LinkingFailed,

        // Preprocessing
        FileNotFound,
        MalformedInclude,
        TokenNotFound,
    };

    struct ShaderManagerData
    {
        Containers::Vectors::GenerationalKey<Core::Renderer::ShaderID> ShaderKey;
    };

    struct ShaderManagerProgramData
    {
        Containers::Vectors::GenerationalKey<Core::Renderer::ShaderProgramID> ProgramKey;
        // TODO: Invalidate this on GC
        Core::Renderer::ShaderProgramID CachedProgramID;
    };

    class ShaderManager
    {
    public:
        explicit ShaderManager(Resources::ResourceManager& resourceManager,
                               const AssetFetcher& assetFetcher);

        std::expected<Containers::Vectors::GenerationalKey<ShaderManagerProgramData>, ShaderManagerError>
        LoadShaderProgram(
            const std::filesystem::path& vertexShaderPath,
            const std::filesystem::path& fragmentShaderPath);

        Core::Renderer::ShaderProgramID GetShaderProgram(
            Containers::Vectors::GenerationalKey<ShaderManagerProgramData> key) const;

        // Engine Internal
    public:
        void RebuildCache();

    private:
        ShaderLoader m_shaderLoader;
        Resources::ResourceManager& m_resourceManager;

    private:
        Containers::Vectors::GenerationalVector<ShaderManagerData> m_shaders;
        Containers::Vectors::GenerationalVector<ShaderManagerProgramData> m_shaderPrograms;

        // Caches
    private:
        // Path -> Shader Key
        Utilities::AssetCache<std::filesystem::path, Containers::Vectors::GenerationalKey<ShaderManagerData>>
        m_shaderCache;
        // Vertex Shader Key, Fragment Shader Key -> Shader Program Key
        Utilities::AssetCache<std::pair<Containers::Vectors::GenerationalKey<ShaderManagerData>,
                                        Containers::Vectors::GenerationalKey<ShaderManagerData>>,
                              Containers::Vectors::GenerationalKey<ShaderManagerProgramData>,
                              Utilities::Hash::PairHash> m_shaderProgramCache;

    private:
        std::expected<Containers::Vectors::GenerationalKey<ShaderManagerData>, ShaderManagerError>
        CreateShader(
            const std::filesystem::path& path, Core::Renderer::ShaderType type);

        std::expected<Containers::Vectors::GenerationalKey<ShaderManagerProgramData>, ShaderManagerError>
        CreateShaderProgram(
            Containers::Vectors::GenerationalKey<ShaderManagerData> vertexShaderKey,
            Containers::Vectors::GenerationalKey<ShaderManagerData> fragmentShaderKey);

    private:
        std::expected<Core::Renderer::ShaderID, ShaderManagerError> GetShaderFromKey(
            Containers::Vectors::GenerationalKey<ShaderManagerData> key) const;

    private:
        Core::Renderer::ShaderProgramID GetInvalidShaderProgramID() const;

    private:
        void UpdateCache(Containers::Vectors::GenerationalKey<ShaderManagerProgramData> key);
    };
}