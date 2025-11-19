//
// Created by Oskar.Norberg on 2025-11-18.
//

#include "Assets/AssetLoaders/ModelLoaders/OBJModelLoader.h"

#include <fstream>
#include <iostream>

#include "Assets/AssetLoaders/ModelLoaders/AssimpModelLoader.h"
#include "Assets/AssetManager/AssetManager.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling::ModelLoading
{
    std::expected<ModelData, ModelLoadingError> OBJModelLoader::LoadModel(
        const std::filesystem::path& modelPath, bool doFlipUVs
    )
    {
        // Parse OBJ File
        const auto modelResources = ParseOBJFile(modelPath);

        // Convert to Engine Native format
        auto modelData = ConvertToMeshData(modelResources, doFlipUVs);

        // Model is unloaded through RAII deconstructor
        return modelData;
    }

    OBJModelResources OBJModelLoader::ParseOBJFile(const std::filesystem::path& path)
    {
        std::ifstream fileStream(path);
        if (!fileStream.is_open())
        {
            RNGO_ASSERT(false && "OBJModelImporter::ParseOBJFile - Failed to open .obj file.");
            // TODO: Load should probably be able to fail.
            return {};
        }

        // TODO: reserve a guesstimated size in the internal vectors
        OBJModelResources modelResources;

        std::string currentLine;
        while (std::getline(fileStream, currentLine))
        {
            std::istringstream iss(currentLine);
            // TODO: Would be nice to avoid heap allocated string here.
            std::string prefix;
            iss >> prefix;

            if (prefix == "v")
            {
                AppendVertices(modelResources, iss);
            }
            if (prefix == "vn")
            {
                AppendNormals(modelResources, iss);
            }
            if (prefix == "vt")
            {
                AppendTextures(modelResources, iss);
            }
            if (prefix == "f")
            {
                AppendFaces(modelResources, iss);
            }
        }
        fileStream.close();

        return modelResources;
    }

    ModelData OBJModelLoader::ConvertToMeshData(const OBJModelResources& modelResources, bool doFlipUVs)
    {
        const auto& vertices = modelResources.vertices;
        const auto& uvs = modelResources.uvs;
        const auto& normals = modelResources.normals;
        const auto& faces = modelResources.faces;

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
                    if (doFlipUVs)
                    {
                        uv.y *= -1.0f;
                    }
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

        return modelData;
    }

    void OBJModelLoader::AppendVertices(
        OBJModelResources& modelResources, std::istringstream& currentLineStream
    )
    {
        glm::vec3 vertex;
        currentLineStream >> vertex.x >> vertex.y >> vertex.z;
        modelResources.vertices.push_back(vertex);
    }

    void OBJModelLoader::AppendTextures(
        OBJModelResources& modelResources, std::istringstream& currentLineStream
    )
    {
        glm::vec2 texture;
        currentLineStream >> texture.x >> texture.y;
        modelResources.uvs.push_back(texture);
    }

    void OBJModelLoader::AppendNormals(
        OBJModelResources& modelResources, std::istringstream& currentLineStream
    )
    {
        glm::vec3 normal;
        currentLineStream >> normal.x >> normal.y >> normal.z;
        modelResources.normals.push_back(normal);
    }

    void OBJModelLoader::AppendFaces(OBJModelResources& modelResources, std::istringstream& currentLineStream)
    {
        std::string faceSpec;
        Face face;

        size_t currentIteration = 0;
        while (currentLineStream >> faceSpec)
        {
            const size_t firstSlash = faceSpec.find('/');
            const size_t secondSlash = faceSpec.find('/', firstSlash + 1);

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
                        face.uvIndices.emplace();
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
                    face.normalIndices.emplace();
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
        modelResources.faces.push_back(face);
    }
}