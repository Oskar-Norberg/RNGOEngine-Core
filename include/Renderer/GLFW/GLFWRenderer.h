//
// Created by Oskar.Norberg on 2025-08-25.
//

#pragma once

#include <string>

#include "Renderer/RenderBase.h"

namespace RNGOEngine::Core::Renderer
{
    class GLFWRenderer : public RenderBase
    {
    public:
        GLFWRenderer();
        void Render(Window::IWindow& window) override;

    public:
        MeshID CreateMesh(std::span<float> vertices, std::span<unsigned> indices) override;
        ShaderID CreateShader(std::string_view vertexSource, std::string_view fragmentSource) override;
        TextureID CreateTexture(unsigned char* data, int width, int height, int nrChannels) override;
        MaterialID CreateMaterial(ShaderID shader) override;

    private:
        struct MeshSpecification
        {
            unsigned int nrOfVertices;
            unsigned int nrOfIndices;
        };

        std::unordered_map<MeshID, MeshSpecification> m_meshSpecifications;
        MaterialID m_nextMaterialID = 0;

        Components::Camera lastCameraProperties;
        glm::mat4 m_projectionMatrix;
        
    private:
        bool CheckCompilationErrors(unsigned int shader);
        bool CheckLinkingErrors(unsigned int program);

    private:
        void RecalculateProjectionMatrix(Components::Camera camera);
    };
}