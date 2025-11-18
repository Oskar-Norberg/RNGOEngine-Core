//
// Created by Oskar.Norberg on 2025-11-04.
//

#include "Assets/Bootstrapper/AssetImporterBootstrapper.h"

#include "Assets/AssetImporters/AssimpModelImporter.h"
#include "Assets/AssetImporters/OBJModelImporter.h"
#include "Assets/AssetImporters/ShaderAssetImporter.h"
#include "Assets/AssetImporters/TextureAssetImporter.h"
#include "Assets/AssetSerializers/ModelAssetSerializer.h"
#include "Assets/AssetSerializers/ShaderAssetSerializer.h"
#include "Assets/AssetSerializers/TextureAssetSerializer.h"

namespace RNGOEngine::AssetHandling
{
    void AssetImporterBootstrapper::Bootstrap(BootstrapContext& context)
    {
        const auto& assetLoader = &context.loader;
        // Loaders
        {
            assetLoader->RegisterImporter<AssimpModelImporter>(AssetType::Model, context.doFlipUVs);
            assetLoader->RegisterImporter<OBJModelImporter>(AssetType::Model);

            assetLoader->RegisterImporter<TextureAssetImporter>(AssetType::Texture);
            assetLoader->RegisterImporter<ShaderAssetImporter>(AssetType::Shader);
        }

        // Serializers
        {
            assetLoader->RegisterSerializer<ModelAssetSerializer>(AssetType::Model);
            assetLoader->RegisterSerializer<TextureAssetSerializer>(AssetType::Texture);
            assetLoader->RegisterSerializer<ShaderAssetSerializer>(AssetType::Shader);
        }
    }
}