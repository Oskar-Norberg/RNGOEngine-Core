//
// Created by Oskar.Norberg on 2025-09-08.
//

#include "AssetHandling/AssetLoaders/ModelLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stack>

#include "Data/MeshData.h"
#include "Renderer/IRenderer.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling::ModelLoading
{
    // Make this static to this specific cpp file. No need to expose it in the header.
    static Data::Rendering::MeshData ProcessMesh(const aiMesh* mesh, const aiScene* scene)
    {
        Data::Rendering::MeshData meshData;

        // Vertex Data
        for (size_t i = 0; i < mesh->mNumVertices; ++i)
        {
            Data::Rendering::Vertex vertex;

            // Positions
            vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};

            // Normals
            if (mesh->HasNormals())
            {
                vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
            }

            const bool hasTextureCoords = mesh->HasTextureCoords(0);
            vertex.texCoord = hasTextureCoords
                                  ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
                                  : glm::vec2(0.0f, 0.0f);

            meshData.vertices.emplace_back(vertex);
        }

        for (size_t i = 0; i < mesh->mNumFaces; ++i)
        {
            const auto* face = &mesh->mFaces[i];

            for (size_t j = 0; j < face->mNumIndices; ++j)
            {
                meshData.indices.emplace_back(face->mIndices[j]);
            }
        }

        return meshData;
    }

    std::expected<ModelHandle, ModelLoadingError> LoadModel(const std::filesystem::path& modelPath,
                                                            const bool doFlipUVs)
    {
        Assimp::Importer importer;

        const auto flags = aiProcess_Triangulate | (doFlipUVs ? aiProcess_FlipUVs : 0) | aiProcess_GenNormals;
        const auto* scene = importer.ReadFile(modelPath.string(), flags);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            RNGO_ASSERT(false && "Failed to load model");
            return std::unexpected(ModelLoadingError::FailedToLoad);
        }

        auto* modelData = new ModelData();
        modelData->meshes.reserve(scene->mNumMeshes);

        std::stack<aiNode*> nodeStack;
        nodeStack.push(scene->mRootNode);

        while (!nodeStack.empty())
        {
            const auto* node = nodeStack.top();
            nodeStack.pop();

            // Enqueue Children
            for (size_t i = 0; i < node->mNumChildren; ++i)
            {
                nodeStack.push(node->mChildren[i]);
            }

            // Process Meshes.
            for (size_t i = 0; i < node->mNumMeshes; ++i)
            {
                modelData->meshes.emplace_back(ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene));
            }
        }

        if (modelData->meshes.empty())
        {
            delete modelData;
            return std::unexpected(ModelLoadingError::NoMeshesFound);
        }

        return ModelHandle{modelData};
    }

    void UnloadModel(const ModelHandle handle)
    {
        delete handle.data;
    }
}