//
// Created by Oskar.Norberg on 2025-09-08.
//

#pragma once

#include <filesystem>

#include "Data/MeshData.h"
#include "Renderer/RenderID.h"

struct aiScene;
struct aiMesh;

namespace RNGOEngine::Core::Renderer
{
    class IRenderer;
}

namespace RNGOEngine::AssetHandling
{
    class ModelLoader
    {
    public:
        explicit ModelLoader(Core::Renderer::IRenderer& renderer);

        Core::Renderer::ModelID LoadModel(const std::filesystem::path& modelPath);

    private:
        Data::Rendering::MeshData ProcessMesh(const aiMesh* mesh, const aiScene* scene);

    private:
        Core::Renderer::IRenderer& m_renderer;
    };
}