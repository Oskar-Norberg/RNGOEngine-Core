//
// Created by Oskar.Norberg on 2025-09-08.
//

#pragma once

#include <expected>
#include <filesystem>

#include "ModelLoaderData.h"

struct aiScene;
struct aiMesh;

namespace RNGOEngine::AssetHandling::ModelLoading
{
    class AssimpModelLoader
    {
    public:
        static std::expected<ModelData, ModelLoadingError> LoadModel(
            const std::filesystem::path& modelPath, bool doFlipUVs
        );

    private:
        static Data::Rendering::MeshData ProcessMesh(const aiMesh* mesh, const aiScene* scene);
    };
}