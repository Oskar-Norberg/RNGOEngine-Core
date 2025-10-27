//
// Created by Oskar.Norberg on 2025-10-27.
//

#pragma once

#include <filesystem>
#include "Utilities/UUID/UUID.h"

namespace RNGOEngine::AssetHandling
{
    class MeshDatabase
    {
        Utilities::UUID LoadModel(const std::filesystem::path& modelPath);
    };
}