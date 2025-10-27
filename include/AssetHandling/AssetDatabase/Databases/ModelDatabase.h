//
// Created by Oskar.Norberg on 2025-10-27.
//

#pragma once

#include <filesystem>

#include "AssetHandling/AssetManager/Managers/ModelManager.h"
#include "Utilities/UUID/UUID.h"

namespace RNGOEngine::AssetHandling
{
    // enum class 
    // struct MeshRecord
    // {
    //     Utilities::UUID uuid;
    //     // MetaData
    //     std::filesystem::path path;
    //     std::optional<Data::Rendering::MeshData> model;
    // };
    
    class ModelDatabase
    {
    public:
        Utilities::UUID LoadModel(const std::filesystem::path& modelPath);

    private:
        
    };
}