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
        TextureHandle CreateTexture(unsigned char* data, int width, int height, int nrChannels) override;

        void SetBool(ShaderHandle shader, std::string_view name, bool value) override;
        void SetInt(ShaderHandle shader, std::string_view name, int value) override;
        void SetFloat(ShaderHandle shader, std::string_view name, float value) override;
        void SetVec2(ShaderHandle shader, std::string_view name, const glm::vec2& value) override;
        void SetVec3(ShaderHandle shader, std::string_view name, const glm::vec3& value) override;
        void SetVec4(ShaderHandle shader, std::string_view name, const glm::vec4& value) override;
        void SetMat4(ShaderHandle shader, std::string_view name, const glm::mat4& value) override;

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