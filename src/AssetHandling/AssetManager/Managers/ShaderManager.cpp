//
// Created by Oskar.Norberg on 2025-09-11.
//

#include "AssetHandling/AssetManager/Managers/ShaderManager.h"

#include "ResourceManager/ResourceManager.h"

namespace RNGOEngine::AssetHandling
{
    ShaderManager::ShaderManager(Resources::ResourceManager& resourceManager,
                                 const AssetFetcher& assetFetcher)
        : m_shaderLoader(assetFetcher),
          m_resourceManager(resourceManager),
          m_shaderProgramCache()
    {
    }

    std::expected<Containers::Vectors::GenerationalKey<ShaderManagerProgramData>, ShaderManagerError>
    ShaderManager::LoadShaderProgram(const std::filesystem::path& vertexShaderPath,
                                     const std::filesystem::path& fragmentShaderPath)
    {
        const auto vertexShaderKey = CreateShader(vertexShaderPath, Core::Renderer::ShaderType::Vertex);
        if (!vertexShaderKey)
        {
            return std::unexpected(vertexShaderKey.error());
        }

        const auto fragmentShaderKey = CreateShader(fragmentShaderPath, Core::Renderer::ShaderType::Fragment);
        if (!fragmentShaderKey)
        {
            return std::unexpected(fragmentShaderKey.error());
        }

        const auto shaderProgram = CreateShaderProgram(vertexShaderKey.value(), fragmentShaderKey.value());
        if (!shaderProgram.has_value())
        {
            return std::unexpected(shaderProgram.error());
        }

        return shaderProgram.value();
    }

    Core::Renderer::ShaderProgramID ShaderManager::GetShaderProgram(
        Containers::Vectors::GenerationalKey<ShaderManagerProgramData> key) const
    {
        if (const auto shaderKey = m_shaderPrograms.GetUnmarkedValidated(key); shaderKey)
        {
            return shaderKey.value().get().CachedProgramID;
        }
        else
        {
            return GetInvalidShaderProgramID();
        }
    }

    void ShaderManager::RebuildCache()
    {
        for (const auto key : m_shaderPrograms.Live())
        {
        }
    }

    std::expected<Containers::Vectors::GenerationalKey<ShaderManagerData>, ShaderManagerError>
    ShaderManager::CreateShader(
        const std::filesystem::path& path, Core::Renderer::ShaderType type)
    {
        // Is already cached?
        if (const auto shader = m_shaderCache.TryGet(path); shader.has_value())
        {
            if (m_shaders.IsValidUnmarked(shader.value()))
            {
                return shader.value();
            }
            else
            {
                // Remove invalid cache entry.
                m_shaderCache.Remove(path);
            }
        }

        const auto shaderSource = m_shaderLoader.LoadShader(path);
        if (!shaderSource.has_value())
        {
            // TODO: Move to separate mapping function.
            switch (shaderSource.error())
            {
                case Shaders::ShaderPreProcessingError::None:
                    return std::unexpected(ShaderManagerError::None);
                case Shaders::ShaderPreProcessingError::FileNotFound:
                    return std::unexpected(ShaderManagerError::FileNotFound);
                case Shaders::ShaderPreProcessingError::MalformedInclude:
                    return std::unexpected(ShaderManagerError::MalformedInclude);
                case Shaders::ShaderPreProcessingError::TokenNotFound:
                    return std::unexpected(ShaderManagerError::TokenNotFound);
            }
        }

        const auto shaderKey = m_resourceManager.CreateShader(shaderSource.value(), type);

        const auto shaderManagerData = ShaderManagerData{.ShaderKey = shaderKey};
        const auto key = m_shaders.Insert(shaderManagerData);

        m_shaderCache.Insert(path, key);

        return key;
    }

    std::expected<Containers::Vectors::GenerationalKey<ShaderManagerProgramData>, ShaderManagerError>
    ShaderManager::CreateShaderProgram(
        Containers::Vectors::GenerationalKey<ShaderManagerData> vertexShaderKey,
        Containers::Vectors::GenerationalKey<ShaderManagerData> fragmentShaderKey)
    {
        const auto pair = std::make_pair(vertexShaderKey, fragmentShaderKey);

        // Is already cached?
        if (const auto shaderProgram = m_shaderProgramCache.TryGet(pair); shaderProgram)
        {
            if (m_shaderPrograms.IsValidUnmarked(shaderProgram.value()))
            {
                return shaderProgram.value();
            }
            else
            {
                // Remove invalid cache entry.
                m_shaderProgramCache.Remove(pair);
            }
        }

        const auto vertexShaderKeyOpt = m_shaders.GetUnmarkedValidated(vertexShaderKey);
        if (!vertexShaderKeyOpt)
        {
            return std::unexpected(ShaderManagerError::ShaderNotFound);
        }
        const auto vertexShader = vertexShaderKeyOpt.value().get().ShaderKey;

        const auto fragmentShaderKeyOpt = m_shaders.GetUnmarkedValidated(fragmentShaderKey);
        if (!fragmentShaderKeyOpt)
        {
            return std::unexpected(ShaderManagerError::ShaderNotFound);
        }
        const auto fragmentShader = fragmentShaderKeyOpt.value().get().ShaderKey;

        const auto key = m_resourceManager.CreateShaderProgram(vertexShader, fragmentShader);
        const auto wrappedKey = m_shaderPrograms.Insert(ShaderManagerProgramData{key});

        UpdateCache(wrappedKey);
        m_shaderProgramCache.Insert(pair, wrappedKey);

        return wrappedKey;
    }

    std::expected<Core::Renderer::ShaderID, ShaderManagerError> ShaderManager::GetShaderFromKey(
        Containers::Vectors::GenerationalKey<ShaderManagerData> key) const
    {
        if (const auto shaderKey = m_shaders.GetUnmarkedValidated(key); shaderKey)
        {
            const auto shaderID = m_resourceManager.GetShader(shaderKey.value().get().ShaderKey);
            if (shaderID)
            {
                return shaderID.value();
            }
        }

        return std::unexpected(ShaderManagerError::ShaderNotFound);
    }

    Core::Renderer::ShaderProgramID ShaderManager::GetInvalidShaderProgramID() const
    {
        // TODO: Return actual invalid shader.
        return Core::Renderer::INVALID_SHADER_PROGRAM_ID;
    }

    void ShaderManager::UpdateCache(Containers::Vectors::GenerationalKey<ShaderManagerProgramData> key)
    {
        const auto reference = m_shaderPrograms.GetUnmarkedValidated(key);
        if (!reference)
        {
            RNGO_ASSERT(false && "ShaderManager::UpdateCache invalid key.");
            return;
        }

        const auto shaderProgramIDOpt = m_resourceManager.
            GetShaderProgram(reference.value().get().ProgramKey);
        if (shaderProgramIDOpt)
        {
            reference.value().get().CachedProgramID = shaderProgramIDOpt.value();
        }
        else
        {
            // TODO: Invalidate shaderProgram??
            RNGO_ASSERT(false && "ShaderManager::UpdateCache invalid key.");
            m_shaderPrograms.Remove(key);
        }
    }
}