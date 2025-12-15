//
// Created by Oskar.Norberg on 2025-11-17.
//

#include "Assets/Builtin/BuiltinAssetBootstrapper.h"

#include "Assets/AssetLoader.h"
#include "Assets/AssetManager/AssetManager.h"
#include "Data/FallbackAssets.h"

namespace RNGOEngine::AssetHandling
{
    AssetHandle BuiltinAssets::s_errorHandles[static_cast<size_t>(AssetType::Count)];

    void BuiltinAssets::InitializeBuiltinAssets()
    {
        SetUpModel();
        SetUpTexture();
        SetUpMaterial();
    }

    AssetHandle BuiltinAssets::GetErrorHandle(AssetType type)
    {
        // TODO: Assert type is valid and has been set up.
        return s_errorHandles[static_cast<size_t>(type)];
    }

    void BuiltinAssets::SetUpModel()
    {
        auto& loader = AssetLoader::GetInstance();

        // Import
        s_errorHandles[static_cast<size_t>(AssetType::Model)] =
            loader.Import(AssetType::Model, Data::FallbackAssets::InvalidModel);

        // Load
        loader.Load(s_errorHandles[static_cast<size_t>(AssetType::Model)]);
    }

    void BuiltinAssets::SetUpTexture()
    {
        auto& loader = AssetLoader::GetInstance();

        // Import
        s_errorHandles[static_cast<size_t>(AssetType::Texture)] =
            loader.Import(AssetType::Texture, Data::FallbackAssets::InvalidTexture);

        // Load
        loader.Load(s_errorHandles[static_cast<size_t>(AssetType::Texture)]);
    }

    void BuiltinAssets::SetUpMaterial()
    {
        auto& materialManager = AssetManager::GetInstance().GetMaterialManager();
        auto& loader = AssetLoader::GetInstance();

        // Import Shader
        const auto fallbackShader = loader.Import(AssetType::Shader, Data::FallbackAssets::InvalidShader);
        s_errorHandles[static_cast<size_t>(AssetType::Shader)] = fallbackShader;
        // Load Shader
        loader.Load(fallbackShader);

        // Create Material
        s_errorHandles[static_cast<size_t>(AssetType::Material)] =
            materialManager.CreateMaterial(fallbackShader).GetMaterialAssetHandle();
    }
}