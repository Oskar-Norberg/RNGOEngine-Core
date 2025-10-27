//
// Created by Oskar.Norberg on 2025-10-24.
//

#pragma once

#include <filesystem>
#include "Utilities/UUID/UUID.h"

namespace RNGOEngine::AssetHandling
{
    class AssetDatabase
    {
    public:
        Utilities::UUID LoadModel(const std::filesystem::path& modelPath);

    private:
        
    };
}