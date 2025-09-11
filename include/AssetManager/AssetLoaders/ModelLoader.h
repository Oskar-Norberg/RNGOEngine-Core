//
// Created by Oskar.Norberg on 2025-09-08.
//

#pragma once

#include <expected>
#include <filesystem>

#include "Data/MeshData.h"

struct aiScene;
struct aiMesh;

namespace RNGOEngine::Core::Renderer
{
    class IRenderer;
}

namespace RNGOEngine::AssetHandling::ModelLoading
{
    struct ModelData
    {
        std::vector<Data::Rendering::MeshData> meshes;
    };

    struct ModelHandle
    {
        // TODO: Again, this should probably be a smart pointer.
        ModelData* data;
    };

    enum class ModelLoadingError
    {
        None,
        FileNotFound,
        FailedToLoad,
        NoMeshesFound,
        UnsupportedFormat,
    };
    
    std::expected<ModelHandle, ModelLoadingError> LoadModel(const std::filesystem::path& modelPath, bool doFlipUVs);
    void UnloadModel(ModelHandle handle);
}