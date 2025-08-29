//
// Created by Oskar.Norberg on 2025-08-27.
//

#pragma once
#include "Renderer/IRenderer.h"

namespace RNGOEngine::Core::Renderer
{
    class NullRenderer : public IRenderer
    {
    public:
        void SubmitDrawQueue(DrawQueue drawQueue) override
        {
        }

        void Render(Window::IWindow& window) override
        {
        }

        MeshID CreateMesh(std::span<float> vertices, std::span<unsigned int> indices) override
        {
            return INVALID_MESH_ID;
        }

        ShaderID CreateShader(std::string_view vertexSource, std::string_view fragmentSource) override
        {
            return INVALID_SHADER_ID;
        }

        MaterialID CreateMaterial(ShaderID shader) override
        {
            return INVALID_MATERIAL_ID;
        }

        TextureID CreateTexture(unsigned char* data, int width, int height, int nrChannels) override
        {
            return INVALID_TEXTURE_ID;
        }

        void SetTexture(MaterialID shader, TextureID texture, int slot) override
        {
        }

        void SetBool(MaterialID shader, std::string_view name, bool value) override
        {
        }

        void SetInt(MaterialID shader, std::string_view name, int value) override
        {
        }

        void SetFloat(MaterialID shader, std::string_view name, float value) override
        {
        }

        void SetVec2(MaterialID shader, std::string_view name, const glm::vec2& value) override
        {
        }

        void SetVec3(MaterialID shader, std::string_view name, const glm::vec3& value) override
        {
        }

        void SetVec4(MaterialID shader, std::string_view name, const glm::vec4& value) override
        {
        }

        void SetMat4(MaterialID shader, std::string_view name, const glm::mat4& value) override
        {
        }

        bool ListenSendEvents(Events::EventQueue& eventQueue) override
        {
            return false;
        }
    };
}