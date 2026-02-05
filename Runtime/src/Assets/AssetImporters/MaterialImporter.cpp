//
// Created by Oskar.Norberg on 2026-02-04.
//

#include "Assets/AssetImporters/MaterialImporter.h"

#include <yaml-cpp/yaml.h>

#include "Assets/AssetLoader.h"
#include "Assets/AssetManager/AssetManager.h"
#include "Utilities/IO/SimpleFileReader/SimpleFileReader.h"

namespace RNGOEngine::AssetHandling
{
    ImportingError MaterialImporter::LoadFromDisk(
        RuntimeAssetRegistry& registry, const AssetMetadata& metadata
    )
    {
        // Read yaml from .mat file
        const auto& path = metadata.Path;
        if (path.empty() || !Utilities::IO::FileExists(path))
        {
            return ImportingError::FileNotFound;
        }
        if (path.extension() != ".mat")
        {
            return ImportingError::UnsupportedFormat;
        }

        YAML::Node node = YAML::LoadFile(path.string());
        const auto shaderUUIDVal = node["Shader"].as<std::uint64_t>();
        const auto albedoUUIDVal = node["Albedo"].as<std::uint64_t>();
        const auto specularUUIDVal = node["Specular"].as<std::uint64_t>();

        const Utilities::UUID shaderUUID = Utilities::UUID(shaderUUIDVal);
        const Utilities::UUID albedoUUID = Utilities::UUID(albedoUUIDVal);
        const Utilities::UUID specularUUID = Utilities::UUID(specularUUIDVal);

        // TODO: Again, lazy load?
        AssetLoader::GetInstance().Load(TextureHandle{albedoUUID});
        AssetLoader::GetInstance().Load(TextureHandle{specularUUID});


        // Shouldn't this just be lazy loaded on first-use?
        AssetLoader::GetInstance().Load(ShaderHandle{shaderUUID});

        // TODO: Read params
        const MaterialSpecification materialSpecification{
            .ShaderHandle = ShaderHandle{shaderUUID},
            .AlbedoTextureHandle = TextureHandle{albedoUUID},
            .SpecularTextureHandle = TextureHandle{specularUUID},
        };

        AssetManager::GetInstance().GetMaterialManager().CreateMaterial(
            AssetHandle{metadata.UUID, AssetType::Material}, materialSpecification
        );

        return ImportingError::None;
    }

    ImportingError MaterialImporter::FinalizeLoad(Data::ThreadType threadType, RuntimeAssetRegistry& registry)
    {
        return ImportingError::None;
    }

    void MaterialImporter::Unload(const AssetHandle& handle)
    {
    }

    std::unique_ptr<AssetMetadata> MaterialImporter::CreateDefaultMetadata(
        const std::filesystem::path& path
    ) const
    {
        auto metadata = std::make_unique<MaterialMetadata>();
        metadata->Path = path;
        metadata->Type = GetAssetType();

        return metadata;
    }

    std::span<const std::string_view> MaterialImporter::GetSupportedExtensions() const
    {
        static constexpr std::array<std::string_view, 1> extensions = {".mat"};

        return extensions;
    }
}