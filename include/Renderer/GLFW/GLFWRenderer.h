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
        VAO CreateVAO() override;
        VBO CreateVBO(size_t size) override;
        EBO CreateEBO(size_t size) override;
        void BindVBOToVAO(VAO vao, VBO vbo) override;
        void BindEBOToVAO(VAO vao, EBO ebo) override;
        void SetAttributePointer(VAO vao, unsigned index, int size, size_t stride, size_t offset) override;
        void BufferVBOData(VBO vbo, std::span<std::byte> data, bool isDynamic) override;
        void BufferEBOData(EBO ebo, std::span<std::byte> data, bool isDynamic) override;
        void DestroyVAO(VAO vao) override;
        void DestroyVBO(VBO vbo) override;
        void DestroyEBO(EBO ebo) override;
        ShaderID CreateShader(std::string_view source, ShaderType type) override;
        TextureID CreateTexture(AssetHandling::Textures::TextureHandle textureHandle) override;
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