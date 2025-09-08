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
        GLFWRenderer(int viewportWidth, int viewportHeight);
        void Render(Window::IWindow& window) override;

    public:
        MeshID CreateMesh(const Data::Rendering::MeshData& meshData) override;
        ShaderID CreateShader(std::string_view source, ShaderType type) override;
        TextureID CreateTexture(unsigned char* data, int width, int height, int nrChannels) override;
        MaterialID CreateMaterial(ShaderProgramID shaderProgramID) override;
        ShaderProgramID CreateShaderProgram(ShaderID vertexShader, ShaderID fragmentShader) override;

        bool ListenSendEvents(Events::EventQueue& eventQueue) override;

    private:
        struct MeshSpecification
        {
            unsigned int nrOfVertices;
            unsigned int nrOfIndices;
        };

        // TODO: Move to RenderBase?
        std::unordered_map<MeshID, MeshSpecification> m_meshSpecifications;
        MaterialID m_nextMaterialID = 0;

        bool m_isProjectionMatrixDirty;
        CameraData m_lastCameraProperties;
        glm::mat4 m_projectionMatrix;

    private:
        int m_viewportWidth, m_viewportHeight;

    private:
        bool CheckCompilationErrors(unsigned int shader);
        bool CheckLinkingErrors(unsigned int program);

    private:
        void RecalculateProjectionMatrix(CameraData camera);
    };
}