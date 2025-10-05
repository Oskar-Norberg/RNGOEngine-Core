//
// Created by Oskar.Norberg on 2025-09-11.
//

#pragma once
#include <expected>
#include <string_view>

#include "Renderer/IRenderer.h"
#include "Renderer/RenderID.h"

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
        CompilationFailed,
        LinkingFailed
    };

    class ShaderManager
    {
    public:
        explicit ShaderManager(Resources::ResourceManager& resourceManager);
        
        std::expected<Core::Renderer::ShaderID, ShaderManagerError> CreateShader(
            std::string_view shaderSource, Core::Renderer::ShaderType type);

        std::expected<Core::Renderer::ShaderProgramID, ShaderManagerError> CreateShaderProgram(
            Core::Renderer::ShaderID vertexShader,
            Core::Renderer::ShaderID fragmentShader);

    private:
        Resources::ResourceManager& m_resourceManager;
    };
}