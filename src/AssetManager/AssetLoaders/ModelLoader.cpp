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

        const auto* scene = importer.ReadFile(modelPath.string(), aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            assert(false && "Failed to load model");
        }

        std::vector<Data::Rendering::MeshData> meshes;

        std::stack<aiNode*> nodeStack;
        nodeStack.push(scene->mRootNode);

        while (!nodeStack.empty())
        {
            const auto* node = nodeStack.top();
            nodeStack.pop();

            Data::Rendering::MeshData meshData;

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

        for (int i = 0; i < mesh->mNumVertices; ++i)
        {
            Data::Rendering::Vertex vertex;
            
            vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
            vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};

            const bool hasTextureCoords = mesh->HasTextureCoords(i);
            vertex.texCoord = hasTextureCoords
                                  ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
                                  : glm::vec2(0.0f, 0.0f);

            meshData.vertices.emplace_back(vertex);
        }

        return meshData;
    }
}