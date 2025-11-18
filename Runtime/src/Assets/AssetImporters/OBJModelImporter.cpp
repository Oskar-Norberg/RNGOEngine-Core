//
// Created by Oskar.Norberg on 2025-11-18.
//

#include <fstream>
#include <iostream>

#include "Assets/AssetImporters/OBJModelImporter.h"
#include "Assets/AssetLoaders/ModelLoader.h"
#include "Assets/AssetManager/AssetManager.h"
#include "Assets/AssetTypes/ModelAsset.h"
#include "Utilities/RNGOAsserts.h"
#include "assimp/Vertex.h"

namespace RNGOEngine::AssetHandling
{
    void OBJModelImporter::Load(const AssetMetadata& metadata)
    {
        const auto& typedMetadata = static_cast<const ModelMetadata&>(metadata);

        std::ifstream fileStream(typedMetadata.Path);
        if (!fileStream.is_open())
        {
            RNGO_ASSERT(false && "ObjModelLoader::Load - Failed to open .obj file.");
            // TODO: Load should probably be able to fail.
            return;
        }

        // TODO: Consider making these static and reusing them?
        // Data
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;

        // Indices
        struct Face
        {
            std::array<Data::Rendering::Index, 3> vertexIndices;
            std::optional<std::array<Data::Rendering::Index, 3>> uvIndices;
            std::optional<std::array<Data::Rendering::Index, 3>> normalIndices;
        };
        std::vector<Face> faces;

        // TODO: reserve a guesstimated size
        std::string currentLine;
        while (std::getline(fileStream, currentLine))
        {
            std::istringstream iss(currentLine);
            // TODO: Would be nice to avoid heap allocated string here.
            std::string prefix;
            iss >> prefix;

            if (prefix == "v")
            {
                glm::vec3 vertex;
                iss >> vertex.x >> vertex.y >> vertex.z;
                vertices.push_back(vertex);
            }
            if (prefix == "vn")
            {
                glm::vec3 normal;
                iss >> normal.x >> normal.y >> normal.z;
                normals.push_back(normal);
            }
            if (prefix == "vt")
            {
                glm::vec2 texture;
                iss >> texture.x >> texture.y;
                uvs.push_back(texture);
            }
            if (prefix == "f")
            {
                std::string faceSpec;
                Face face;

                size_t currentIteration = 0;
                while (iss >> faceSpec)
                {
                    size_t firstSlash = faceSpec.find('/');
                    size_t secondSlash = faceSpec.find('/', firstSlash + 1);

                    // V
                    const std::string vertexIndexStr = faceSpec.substr(0, firstSlash);
                    const int vertexIndex = std::stoi(vertexIndexStr);
                    face.vertexIndices[currentIteration] = vertexIndex;

                    // VT
                    if (firstSlash != std::string::npos && secondSlash != std::string::npos)
                    {
                        const std::string texCoordIndexStr =
                            faceSpec.substr(firstSlash + 1, secondSlash - firstSlash - 1);
                        if (!texCoordIndexStr.empty())
                        {
                            const int texCoordIndex = std::stoi(texCoordIndexStr);
                            if (!face.uvIndices)
                            {
                                face.uvIndices = std::array<Data::Rendering::Index, 3>();
                            }
                            face.uvIndices.value()[currentIteration] = texCoordIndex;
                        }
                    }

                    // VN
                    if (secondSlash != std::string::npos)
                    {
                        const std::string normalIndexStr = faceSpec.substr(secondSlash + 1);
                        const int normalIndex = std::stoi(normalIndexStr);
                        if (!face.normalIndices)
                        {
                            face.normalIndices = std::array<Data::Rendering::Index, 3>();
                        }
                        face.normalIndices.value()[currentIteration] = normalIndex;
                    }

                    currentIteration++;
                }

                if (currentIteration != 3)
                {
                    RNGO_ASSERT(false && "ObjModelLoader::Load - Only triangular faces are supported.");
                    return;
                }

                // Add indices
                faces.push_back(face);
            }
        }
        fileStream.close();

        // For now, only support single mesh obj files.
        ModelLoading::ModelData modelData;
        auto& meshData = modelData.meshes.emplace_back();
        meshData.vertices.resize(vertices.size());
        meshData.indices.reserve(faces.size() * 3);

        // Convert to MeshData
        for (const auto& face : faces)
        {
            for (size_t i = 0; i < 3; ++i)
            {
                Data::Rendering::Vertex vertex{};
                const auto vertIndex = face.vertexIndices[i] - 1;
                vertex.position = vertices[vertIndex];

                if (face.uvIndices)
                {
                    const auto uvIndex = face.uvIndices.value()[i] - 1;
                    // Note: Flip UVs vertically for OpenGL
                    glm::vec2 uv = uvs[uvIndex];
                    uv.y *= -1.0f;
                    vertex.texCoord = uv;
                }

                if (face.normalIndices)
                {
                    const auto normalIndex = face.normalIndices.value()[i] - 1;
                    vertex.normal = normals[normalIndex];
                }

                meshData.vertices.at(vertIndex) = vertex;
                meshData.indices.push_back(static_cast<Data::Rendering::Index>(vertIndex));
            }
        }

        ModelLoading::ModelHandle modelHandle{&modelData};
        const auto errorMessage =
            AssetManager::GetInstance().GetModelManager().UploadModel(typedMetadata.UUID, modelHandle);

        if (errorMessage != ModelCreationError::None)
        {
            RNGO_ASSERT(false && "ObjModelLoader::Load - Failed to Load Model");
            return;
        }

        return;
    }

    void OBJModelImporter::Unload(const AssetHandle& handle)
    {
    }

    std::unique_ptr<AssetMetadata> OBJModelImporter::CreateDefaultMetadata(
        const std::filesystem::path& path
    ) const
    {
        auto modelData = std::make_unique<ModelMetadata>();
        modelData->Type = AssetType::Model;
        return std::move(modelData);
    }

    std::span<const std::string_view> OBJModelImporter::GetSupportedExtensions() const
    {
        static constexpr std::string_view supportedTypes[] = {
            ".obj",
        };

        return supportedTypes;
    }
}