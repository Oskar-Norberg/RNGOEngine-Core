//
// Created by Oskar.Norberg on 2025-08-25.
//

#include "Renderer/GLFW/GLFWRenderer.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

// TODO: AAAAAAAAAAAAAAAAAAAAA
#include <cassert>

#include "stb_image.h"

namespace RNGOEngine::Core::Renderer
{
    GLFWRenderer::GLFWRenderer()
    {
        if (!gladLoadGL(glfwGetProcAddress))
        {
            assert(false && "Failed to initialize GLAD");
        }

        // Not 100% sure if this should be done here. But the renderer coordinate system is what decides if a texture needs flipping or not.
        stbi_set_flip_vertically_on_load(true);

        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, 800, 600);
    }

    void GLFWRenderer::Render(Window::IWindow& window)
    {
        glClearColor(0.25f, 0.35f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render Opaques
        for (const auto& opaqueDrawable : m_drawQueue.opaqueObjects)
        {
            const auto& materialSpecification = m_materials[opaqueDrawable.material];
            const auto shaderID = materialSpecification.shader;
            glUseProgram(materialSpecification.shader);
            glBindVertexArray(opaqueDrawable.mesh);

            for (const auto& uniformSpecification : materialSpecification.uniforms)
            {
                switch (uniformSpecification.type)
                {
                    case Bool:
                        glUniform1i(glGetUniformLocation(shaderID, uniformSpecification.name.c_str()),
                                    static_cast<int>(uniformSpecification.data.b));
                        break;
                    case Int:
                        glUniform1i(glGetUniformLocation(shaderID, uniformSpecification.name.c_str()),
                                    uniformSpecification.data.b);
                        break;
                    case Float:
                        glUniform1f(glGetUniformLocation(shaderID, uniformSpecification.name.c_str()),
                                    uniformSpecification.data.f);
                        break;
                    case Vec2:
                        glUniform2fv(glGetUniformLocation(shaderID, uniformSpecification.name.c_str()), 1,
                                     &uniformSpecification.data.v2[0]);
                        break;
                    case Vec3:
                        glUniform3fv(glGetUniformLocation(shaderID, uniformSpecification.name.c_str()), 1,
                                     &uniformSpecification.data.v3[0]);
                        break;
                    case Vec4:
                        glUniform4fv(glGetUniformLocation(shaderID, uniformSpecification.name.c_str()), 1,
                                     &uniformSpecification.data.v4[0]);
                        break;
                    case Mat4:
                        glUniformMatrix4fv(glGetUniformLocation(shaderID, uniformSpecification.name.c_str()),
                                           1, GL_FALSE, &uniformSpecification.data.m4[0][0]);
                        break;
                    case Texture:
                    {
                        const auto uniformLocation = glGetUniformLocation(
                            shaderID, uniformSpecification.name.c_str());
                        if (uniformLocation == -1)
                        {
                            assert(false && "Texture Uniform not found in shader.");
                            break;
                        }

                        glActiveTexture(GL_TEXTURE0 + uniformSpecification.data.texture.slot);
                        glBindTexture(GL_TEXTURE_2D, uniformSpecification.data.texture.texture);
                        glUniform1i(uniformLocation, uniformSpecification.data.texture.slot);
                        break;
                    }

                    default:
                        break;
                }
            }

            glUniformMatrix4fv(glGetUniformLocation(shaderID, "Model"), 1, GL_FALSE,
                               &opaqueDrawable.transform[0][0]);

            assert(m_meshSpecifications.contains(opaqueDrawable.mesh) && "Mesh not found in specifications");

            const auto& meshSpec = m_meshSpecifications[opaqueDrawable.mesh];
            glDrawElements(GL_TRIANGLES, meshSpec.nrOfVertices + meshSpec.nrOfIndices, GL_UNSIGNED_INT, 0);
        }
    }

    MeshID GLFWRenderer::CreateMesh(std::span<float> vertices, std::span<unsigned> indices)
    {
        unsigned int VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), GL_STATIC_DRAW);

        // Vertex Pos
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void*>(nullptr));
        glEnableVertexAttribArray(0);

        // Vertex UV
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        m_meshSpecifications.insert(
            std::make_pair(VAO, MeshSpecification
                           {
                               .nrOfVertices = static_cast<unsigned int>(vertices.size() / 3),
                               .nrOfIndices = static_cast<unsigned int>(indices.size() / 3)
                           }
            ));

        return VAO;
    }

    ShaderID GLFWRenderer::CreateShader(std::string_view vertexSource, std::string_view fragmentSource)
    {
        //  Compile Vertex Shader
        auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* vertexSourcePtr = vertexSource.data();
        glShaderSource(vertexShader, 1, &vertexSourcePtr, nullptr);
        glCompileShader(vertexShader);
        CheckCompilationErrors(vertexShader);

        // Compile Fragment Shader
        auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragmentSourcePtr = fragmentSource.data();
        glShaderSource(fragmentShader, 1, &fragmentSourcePtr, nullptr);
        glCompileShader(fragmentShader);
        CheckCompilationErrors(fragmentShader);

        auto shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        CheckLinkingErrors(shaderProgram);

        return shaderProgram;
    }

    TextureID GLFWRenderer::CreateTexture(unsigned char* data, int width, int height, int nrChannels)
    {
        unsigned int textureHandle;
        glGenTextures(1, &textureHandle);
        glBindTexture(GL_TEXTURE_2D, textureHandle);

        // TODO: These should probably be passed in as parameters.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (nrChannels == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        else if (nrChannels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            assert(false && "Unsupported number of channels in texture");
            return INVALID_TEXTURE_ID;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        return textureHandle;
    }

    MaterialID GLFWRenderer::CreateMaterial(ShaderID shader)
    {
        const auto materialID = m_nextMaterialID++;
        m_materials[materialID] = MaterialSpecification{.shader = shader, .uniforms = {}};

        return materialID;
    }

    bool GLFWRenderer::CheckCompilationErrors(unsigned int shader)
    {
        int success;
        char infoLog[512];

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            assert(false && infoLog);
            return false;
        }

        return true;
    }

    bool GLFWRenderer::CheckLinkingErrors(unsigned int program)
    {
        int success;
        char infoLog[512];

        glGetShaderiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            assert(false && infoLog);
            return false;
        }

        return true;
    }
}