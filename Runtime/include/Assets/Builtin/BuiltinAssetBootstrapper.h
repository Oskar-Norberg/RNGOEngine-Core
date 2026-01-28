//
// Created by Oskar.Norberg on 2025-11-17.
//

#pragma once
#include "Assets/Asset.h"

namespace RNGOEngine::AssetHandling
{
    class BuiltinAssets
    {
    public:
        // TODO: Send parameters instead of relying on singletons?
        static void InitializeBuiltinAssets();
        static AssetHandle GetErrorHandle(AssetType type);

    private:
        static void LoadBuiltin();

    private:
        static void SetUpModel();
        static void SetUpTexture();
        static void SetUpMaterial();

    private:
        static AssetHandle s_errorHandles[AssetTypeCount];
    };
}