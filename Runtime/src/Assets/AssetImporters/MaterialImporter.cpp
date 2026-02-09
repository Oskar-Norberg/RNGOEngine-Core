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

        const auto shaderUUID = Utilities::UUID(shaderUUIDVal);
        // TODO: Hardcoded albedo / specular for now
        const auto albedoUUID = Utilities::UUID(albedoUUIDVal);
        const auto specularUUID = Utilities::UUID(specularUUIDVal);

        // TODO: Again, lazy load?
        AssetLoader::GetInstance().Load(TextureHandle{albedoUUID});
        AssetLoader::GetInstance().Load(TextureHandle{specularUUID});
        AssetLoader::GetInstance().Load(ShaderHandle{shaderUUID});

        MaterialParameters parameters{};
        parameters.Add(
            Data::Shader::ALBEDO_TEXTURE.Value,
            MaterialTextureSpecification{
                .TextureHandle = TextureHandle{albedoUUID},
                .Slot = Data::Shader::ALBEDO_TEXTURE_SLOT,
            }
        );
        parameters.Add(
            Data::Shader::SPECULAR_TEXTURE.Value,
            MaterialTextureSpecification{
                .TextureHandle = TextureHandle{specularUUID},
                .Slot = Data::Shader::SPECULAR_TEXTURE_SLOT,
            }
        );
        const MaterialSpecification materialSpecification{
            .ShaderHandle = ShaderHandle{shaderUUID}, .Parameters = parameters
        };

        AssetManager::GetInstance().GetMaterialManager().CreateMaterial(
            MaterialHandle{metadata.UUID}, materialSpecification
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