//
// Created by Oskar.Norberg on 2025-11-17.
//

#include "Assets/Builtin/BuiltinAssetBootstrapper.h"

#include "Assets/AssetLoader.h"
#include "Assets/AssetManager/AssetManager.h"
#include "Data/FallbackAssets.h"

namespace RNGOEngine::AssetHandling
{
    void BuiltinAssets::InitializeBuiltinAssets()
    {
        SetUpModel();
        SetUpTexture();
        SetUpMaterial();
    }

    AssetHandle BuiltinAssets::GetErrorHandle(AssetType type)
    {
        const auto& assetManager = AssetManager::GetInstance();
        switch (type)
        {
            case AssetType::Model:
                return assetManager.GetModelManager().GetErrorModel();
            case AssetType::Texture:
                return assetManager.GetTextureManager().GetInvalidTexture();
            case AssetType::Shader:
                RNGO_ASSERT(false && "Unsupported type!");
            case AssetType::Material:
                return assetManager.GetMaterialManager().GetInvalidMaterial();
            case AssetType::Count:
            case AssetType::None:
            default:
                RNGO_ASSERT(false && "Unknown AssetType!");
        }
    }

    void BuiltinAssets::SetUpModel()
    {
        auto& modelManager = AssetManager::GetInstance().GetModelManager();
        auto& loader = AssetLoader::GetInstance();

        modelManager.SetErrorModel(loader.Load(AssetType::Model, Data::FallbackAssets::InvalidModel));
    }

    void BuiltinAssets::SetUpTexture()
    {
        auto& textureManager = AssetManager::GetInstance().GetTextureManager();
        auto& loader = AssetLoader::GetInstance();

        textureManager.SetInvalidTexture(loader.Load(AssetType::Texture, Data::FallbackAssets::InvalidTexture)
        );
    }

    void BuiltinAssets::SetUpMaterial()
    {
        auto& materialManager = AssetManager::GetInstance().GetMaterialManager();
        auto& loader = AssetLoader::GetInstance();

        const auto vertexShader = loader.Load(AssetType::Shader, Data::FallbackAssets::InvalidVertexShader);
        const auto fragmentShader =
            loader.Load(AssetType::Shader, Data::FallbackAssets::InvalidFragmentShader);

        const auto invalidMaterialHandle = materialManager.CreateMaterial(vertexShader, fragmentShader);
        materialManager.SetInvalidMaterial(invalidMaterialHandle.GetMaterialAssetHandle());
    }
}