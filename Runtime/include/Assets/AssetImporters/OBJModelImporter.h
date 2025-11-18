//
// Created by Oskar.Norberg on 2025-11-18.
//

#pragma once

#include "AssetImporter.h"
#include "Assets/AssetLoaders/ModelLoader.h"
#include "Data/MeshData.h"

namespace RNGOEngine::AssetHandling
{
    class OBJModelImporter : public AssetImporter
    {
    private:
        struct Face
        {
            std::array<Data::Rendering::Index, 3> vertexIndices;
            std::optional<std::array<Data::Rendering::Index, 3>> uvIndices;
            std::optional<std::array<Data::Rendering::Index, 3>> normalIndices;
        };

        struct ModelResources
        {
            std::vector<glm::vec3> vertices;
            std::vector<glm::vec2> uvs;
            std::vector<glm::vec3> normals;

            std::vector<Face> faces;
        };

    public:
        void Load(const AssetMetadata& metadata) override;

        void Unload(const AssetHandle& handle) override;

        std::unique_ptr<AssetMetadata> CreateDefaultMetadata(
            const std::filesystem::path& path
        ) const override;

        std::span<const std::string_view> GetSupportedExtensions() const override;

    private:
        ModelResources ParseOBJFile(const std::filesystem::path& path);
        ModelLoading::ModelData ConvertToMeshData(const ModelResources& modelResources);

    private:
        void AppendVertices(ModelResources& modelResources, std::istringstream& currentLineStream);
        void AppendTextures(ModelResources& modelResources, std::istringstream& currentLineStream);
        void AppendNormals(ModelResources& modelResources, std::istringstream& currentLineStream);

    private:
        void AppendFaces(ModelResources& modelResources, std::istringstream& currentLineStream);
    };
}