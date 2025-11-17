//
// Created by Oskar.Norberg on 2025-11-17.
//

#include "Assets/Builtin/BuiltinAssetBootstrapper.h"

#include "Assets/AssetLoader.h"
#include "Assets/AssetManager/AssetManager.h"
#include "Data/FallbackAssets.h"

namespace RNGOEngine::AssetHandling
{
    inline void SetUpModel()
    {
        auto& modelManager = AssetManager::GetInstance().GetModelManager();
        auto& loader = AssetLoader::GetInstance();

        modelManager.SetErrorModel(loader.Load(AssetType::Model, Data::FallbackAssets::InvalidModel));
    }

    void InitializeBuiltinAssets()
    {
        SetUpModel();
    }
}