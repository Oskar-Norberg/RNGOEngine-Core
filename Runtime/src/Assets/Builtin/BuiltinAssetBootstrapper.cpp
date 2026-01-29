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
        LoadBuiltin();

        SetUpModel();
        SetUpTexture();
        SetUpMaterial();
    }

    AssetHandle BuiltinAssets::GetErrorHandle(AssetType type)
    {
        // TODO: Assert type is valid and has been set up.
        return s_errorHandles[static_cast<size_t>(type)];
    }

    void BuiltinAssets::LoadBuiltin()
    {
        auto& loader = AssetLoader::GetInstance();

        for (const auto& [type, path] : Data::Assets::AllBuiltinAssets)
        {
            const AssetHandle handle = loader.Import(type, path);
            loader.Load(handle);
        }
    }

    void BuiltinAssets::SetUpModel()
    {
        auto& loader = AssetLoader::GetInstance();
        const AssetHandle handle = loader.Import(AssetType::Model, Data::Assets::InvalidModel.Path);
        s_errorHandles[static_cast<size_t>(AssetType::Model)] = handle;
    }

    void BuiltinAssets::SetUpTexture()
    {
        auto& loader = AssetLoader::GetInstance();
        const AssetHandle handle = loader.Import(AssetType::Texture, Data::Assets::InvalidTexture.Path);
        s_errorHandles[static_cast<size_t>(AssetType::Texture)] = handle;
    }

    void BuiltinAssets::SetUpMaterial()
    {
        auto& materialManager = AssetManager::GetInstance().GetMaterialManager();
        auto& loader = AssetLoader::GetInstance();

        // Import Shader
        const AssetHandle fallbackShader = loader.Import(AssetType::Shader, Data::Assets::InvalidShader.Path);
        s_errorHandles[static_cast<size_t>(AssetType::Shader)] = fallbackShader;

        // Create Material
        // TODO: shit ass material system
        auto handle = Utilities::GenerateUUID();
        MaterialParameters params;
        materialManager.CreateMaterial(MaterialHandle{handle}, fallbackShader, params);
        s_errorHandles[static_cast<size_t>(AssetType::Material)] = MaterialHandle{handle};
    }
}