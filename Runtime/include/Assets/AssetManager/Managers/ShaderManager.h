//
// Created by Oskar.Norberg on 2025-09-11.
//

#pragma once

#include <expected>

#include "Assets/AssetLoaders/ShaderLoader.h"
#include "Renderer/RenderID.h"
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
    using ShaderProgramKey = Containers::GenerationalKey<Core::Renderer::ShaderProgramID>;
    enum class ShaderManagerError
    {
        None,
        // TODO:
    };

    class ShaderManager
    {
    public:
        explicit ShaderManager(Resources::ResourceManager& resourceManager);

    public:
        std::expected<ShaderProgramKey, ShaderManagerError> UploadShader(
            std::string_view vertexShader, std::string_view fragmentShader
        );
        void DestroyShader(ShaderProgramKey shaderAsset);

    private:
        Containers::GenerationalKey<Core::Renderer::ShaderID> CreateShader(
            std::string_view shaderSource, Core::Renderer::ShaderType type
        );

        Containers::GenerationalKey<Core::Renderer::ShaderProgramID> CreateShaderProgram(
            Containers::GenerationalKey<Core::Renderer::ShaderID> vertexShaderKey,
            Containers::GenerationalKey<Core::Renderer::ShaderID> fragmentShaderKey
        );

    private:
        Resources::ResourceManager& m_resourceManager;
    };
}