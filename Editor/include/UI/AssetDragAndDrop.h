//
// Created by Oskar.Norberg on 2025-12-12.
//

#pragma once

#include <filesystem>

#include "Assets/Asset.h"

namespace RNGOEngine::Editor
{
    constexpr auto AssetDragAndDropName = "AssetDragAndDrop";
    
    struct AssetDragAndDropPayload
    {
        AssetHandling::AssetType type;
        std::filesystem::path AssetPath;
    };
}