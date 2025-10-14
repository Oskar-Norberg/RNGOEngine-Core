//
// Created by Oskar.Norberg on 2025-10-14.
//

#pragma once

#include "Renderer/IRenderer.h"

namespace RNGOEngine::Resources
{
    class ShaderResourceManager
    {
    public:
        explicit ShaderResourceManager(RNGOEngine::Core::Renderer::IRenderer& renderer);

    public:
        Core::Renderer::ShaderID CreateShader(std::string_view source, Core::Renderer::ShaderType type);
        Core::Renderer::ShaderProgramID CreateShaderProgram(Core::Renderer::ShaderID vertexShader,
                                                            Core::Renderer::ShaderID fragmentShader);
        
        void DestroyShader(Core::Renderer::ShaderID shader);
        void DestroyShaderProgram(Core::Renderer::ShaderProgramID program);

    private:
        RNGOEngine::Core::Renderer::IRenderer& m_renderer;
    };
}