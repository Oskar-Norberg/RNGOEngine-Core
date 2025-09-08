//
// Created by Oskar.Norberg on 2025-09-08.
//

#include "AssetManager/AssetLoaders/ModelLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <cassert>
#include <stack>

#include "Data/MeshData.h"
#include "Renderer/IRenderer.h"

namespace RNGOEngine::AssetHandling
{
    ModelLoader::ModelLoader(Core::Renderer::IRenderer& renderer, bool doflipUVs)
        : m_renderer(renderer), m_doFlipUVs(doflipUVs)
    {
    }

    std::vector<Core::Renderer::MeshID> ModelLoader::LoadModel(const std::filesystem::path& modelPath)
    {
        Assimp::Importer importer;

        const auto flags = aiProcess_Triangulate | (m_doFlipUVs ? aiProcess_FlipUVs : 0);
        const auto* scene = importer.ReadFile(modelPath.string(), flags);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            assert(false && "Failed to load model");
            // TODO: Better error handling
            return {};
        }

        std::vector<Data::Rendering::MeshData> meshes;

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
                meshes.emplace_back(ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene));
            }
        }

        std::vector<Core::Renderer::MeshID> meshIDs;
        meshIDs.reserve(meshes.size());
        for (const auto& mesh : meshes)
        {
            meshIDs.emplace_back(m_renderer.CreateMesh(mesh));
        }

        return meshIDs;
    }

    Data::Rendering::MeshData ModelLoader::ProcessMesh(const aiMesh* mesh, const aiScene* scene)
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
}