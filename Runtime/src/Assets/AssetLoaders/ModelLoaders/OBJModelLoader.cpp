//
// Created by Oskar.Norberg on 2025-11-18.
//

#include "Assets/AssetLoaders/ModelLoaders/OBJModelLoader.h"

#include <fstream>

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
            // TODO: Parse should return expected with error.
            return {};
        }

        // TODO: reserve a guesstimated size in the internal vectors
        OBJModelResources modelResources;

        std::string currentLine;
        while (std::getline(fileStream, currentLine))
        {
            std::istringstream iss(currentLine);
            // TODO: Would be nice to avoid heap allocated string here. Else just clear and reuse it.
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
        auto& [vertices, uvs, normals, faces] = modelResources;

        // For now, only support single mesh obj files.
        ModelData modelData;
        auto& meshData = modelData.meshes.emplace_back();
        meshData.Vertices.resize(vertices.size());
        meshData.Indices.reserve(faces.size() * 3);

        // Convert to MeshData
        for (const auto& face : faces)
        {
            for (size_t i = 0; i < 3; ++i)
            {
                Data::Rendering::Vertex vertex{};
                const auto vertIndex = face.VertexIndices[i] - 1;
                vertex.Position = vertices[vertIndex];

                if (face.UVIndices)
                {
                    const auto uvIndex = face.UVIndices.value()[i] - 1;
                    // Note: Flip UVs vertically for OpenGL
                    glm::vec2 uv = uvs[uvIndex];
                    if (doFlipUVs)
                    {
                        uv.y *= -1.0f;
                    }
                    vertex.TexCoord = uv;
                }

                if (face.NormalIndices)
                {
                    const auto normalIndex = face.NormalIndices.value()[i] - 1;
                    vertex.Normal = normals[normalIndex];
                }

                meshData.Vertices.at(vertIndex) = vertex;
                meshData.Indices.push_back(static_cast<Data::Rendering::Index>(vertIndex));
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
        modelResources.Vertices.push_back(vertex);
    }

    void OBJModelLoader::AppendTextures(
        OBJModelResources& modelResources, std::istringstream& currentLineStream
    )
    {
        glm::vec2 texture;
        currentLineStream >> texture.x >> texture.y;
        modelResources.UVs.push_back(texture);
    }

    void OBJModelLoader::AppendNormals(
        OBJModelResources& modelResources, std::istringstream& currentLineStream
    )
    {
        glm::vec3 normal;
        currentLineStream >> normal.x >> normal.y >> normal.z;
        modelResources.Normals.push_back(normal);
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
            face.VertexIndices[currentIteration] = vertexIndex;

            // VT
            if (firstSlash != std::string::npos && secondSlash != std::string::npos)
            {
                const std::string texCoordIndexStr =
                    faceSpec.substr(firstSlash + 1, secondSlash - firstSlash - 1);
                if (!texCoordIndexStr.empty())
                {
                    const int texCoordIndex = std::stoi(texCoordIndexStr);
                    if (!face.UVIndices)
                    {
                        face.UVIndices.emplace();
                    }
                    face.UVIndices.value()[currentIteration] = texCoordIndex;
                }
            }

            // VN
            if (secondSlash != std::string::npos)
            {
                const std::string normalIndexStr = faceSpec.substr(secondSlash + 1);
                const int normalIndex = std::stoi(normalIndexStr);
                if (!face.NormalIndices)
                {
                    face.NormalIndices.emplace();
                }
                face.NormalIndices.value()[currentIteration] = normalIndex;
            }

            currentIteration++;
        }

        if (currentIteration != 3)
        {
            RNGO_ASSERT(false && "ObjModelLoader::Load - Only triangular faces are supported.");
            return;
        }

        // Add indices
        modelResources.Faces.push_back(face);
    }
}