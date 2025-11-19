//
// Created by Oskar.Norberg on 2025-11-19.
//

#pragma once

#include "Data/MeshData.h"

namespace RNGOEngine::AssetHandling::ModelLoading
{
    struct ModelData
    {
        std::vector<Data::Rendering::MeshData> meshes;
    };

    enum class ModelLoadingError
    {
        None,
        FileNotFound,
        FailedToLoad,
        NoMeshesFound,
        UnsupportedFormat,
    };
}