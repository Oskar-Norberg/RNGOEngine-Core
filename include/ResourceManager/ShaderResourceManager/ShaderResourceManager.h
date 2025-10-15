//
// Created by Oskar.Norberg on 2025-10-14.
//

#pragma once

#include <string_view>
#include "Renderer/RenderID.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

namespace RNGOEngine::Core::Renderer
{
    class IRenderer;
}

namespace RNGOEngine::Resources
{
    // TODO: Wrap everything in generational handles.
    class ShaderResourceManager
    {
    public:
        explicit ShaderResourceManager(RNGOEngine::Core::Renderer::IRenderer& renderer);

    public:
        Containers::Vectors::GenerationalKey<Core::Renderer::ShaderID> CreateShader(
            std::string_view source, Core::Renderer::ShaderType type);
        Containers::Vectors::GenerationalKey<Core::Renderer::ShaderProgramID> CreateShaderProgram(
            Containers::Vectors::GenerationalKey<Core::Renderer::ShaderID> vertexShader,
            Containers::Vectors::GenerationalKey<Core::Renderer::ShaderID> fragmentShader);

        void MarkShaderForDestruction(Containers::Vectors::GenerationalKey<Core::Renderer::ShaderID> shader);
        void MarkShaderProgramForDestruction(
            Containers::Vectors::GenerationalKey<Core::Renderer::ShaderProgramID> program);

        std::optional<Core::Renderer::ShaderID> GetShader(
            const Containers::Vectors::GenerationalKey<Core::Renderer::ShaderID>& key) const;
        std::optional<Core::Renderer::ShaderProgramID> GetShaderProgram(
            const Containers::Vectors::GenerationalKey<Core::Renderer::ShaderProgramID>& key) const;

    private:
        Containers::Vectors::GenerationalVector<Core::Renderer::ShaderID> m_shaders;
        Containers::Vectors::GenerationalVector<Core::Renderer::ShaderProgramID> m_shaderPrograms;

    private:
        RNGOEngine::Core::Renderer::IRenderer& m_renderer;
    };
}