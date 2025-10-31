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

        // Resources
        ShaderNotFound,
        CompilationFailed,
        LinkingFailed,

        // Preprocessing
        FileNotFound,
        MalformedInclude,
        TokenNotFound,
    };

    struct RuntimeShaderData
    {
        Core::Renderer::ShaderType Type;
        Containers::GenerationalKey<Core::Renderer::ShaderID> ShaderKey;
    };

    struct RuntimeShaderProgramData
    {
        Containers::GenerationalKey<Core::Renderer::ShaderProgramID> ProgramKey;
    };

    // TODO: Separate Shader and ShaderProgram into separate managers.
    class ShaderManager
    {
    public:
        explicit ShaderManager(AssetDatabase& assetDatabase,
                               Resources::ResourceManager& resourceManager,
                               const AssetFetcher& assetFetcher);

    public:
        AssetHandle CreateShader(const std::filesystem::path& path, Core::Renderer::ShaderType type);

    public:
        Containers::GenerationalKey<RuntimeShaderProgramData> CreateShaderProgram(
            const AssetHandle& vertexShader,
            const AssetHandle& fragmentShader);

    public:
        Core::Renderer::ShaderProgramID GetShaderProgram(
            const Containers::GenerationalKey<RuntimeShaderProgramData>& key);

        // Engine Internal
    public:
        void BeginDestroyAllShaders();
        void BeginDestroyAllShaderPrograms();

    private:
        AssetDatabase& m_assetDatabase;
        Resources::ResourceManager& m_resourceManager;
        ShaderLoader m_shaderLoader;

    private:
        Containers::GenerationalVector<RuntimeShaderData> m_shaders;
        Containers::GenerationalVector<RuntimeShaderProgramData> m_shaderPrograms;

    private:
        // TODO: Why exactly is this map pointing to a genkey instead of the actual data?
        std::unordered_map<AssetHandle, Containers::GenerationalKey<RuntimeShaderData>>
        m_handleToShader;
    };
}