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
        s_errorHandles[static_cast<size_t>(AssetType::Model)] =
            loader.Load(AssetType::Model, Data::FallbackAssets::InvalidModel);
    }

    void BuiltinAssets::SetUpTexture()
    {
        auto& loader = AssetLoader::GetInstance();
        s_errorHandles[static_cast<size_t>(AssetType::Texture)] =
            loader.Load(AssetType::Texture, Data::FallbackAssets::InvalidTexture);
    }

    void BuiltinAssets::SetUpMaterial()
    {
        auto& materialManager = AssetManager::GetInstance().GetMaterialManager();
        auto& loader = AssetLoader::GetInstance();

        const auto vertexShader = loader.Load(AssetType::Shader, Data::FallbackAssets::InvalidVertexShader);
        const auto fragmentShader =
            loader.Load(AssetType::Shader, Data::FallbackAssets::InvalidFragmentShader);

        s_errorHandles[static_cast<size_t>(AssetType::Material)] =
            materialManager.CreateMaterial(vertexShader, fragmentShader).GetMaterialAssetHandle();
    }
}