//
// Created by Oskar.Norberg on 2025-08-25.
//

#pragma once

#include "Renderer/RenderBase.h"

namespace RNGOEngine::Core::Renderer
{
    class GLFWRenderer : public RenderBase
    {
    public:
        GLFWRenderer();
        void Render(Window::IWindow& window) override;

    public:
        MeshHandle CreateMesh(std::span<float> vertices, std::span<unsigned> indices) override;
        ShaderHandle CreateShader(std::string_view vertexSource, std::string_view fragmentSource) override;

    private:
        bool CheckCompilationErrors(unsigned int shader);
        bool CheckLinkingErrors(unsigned int program);

    private:
        struct MeshSpecification
        {
            unsigned int nrOfVertices;
            unsigned int nrOfIndices;
        };

        std::unordered_map<MeshHandle, MeshSpecification> m_meshSpecifications;
    };
}