//
// Created by Oskar.Norberg on 2025-11-18.
//

#pragma once

#include "Assets/AssetImporters/AssetImporter.h"
#include "AssimpModelLoader.h"
#include "Data/MeshData.h"

namespace RNGOEngine::AssetHandling::ModelLoading
{
    struct Face
    {
        std::array<Data::Rendering::Index, 3> vertexIndices;
        std::optional<std::array<Data::Rendering::Index, 3>> uvIndices;
        std::optional<std::array<Data::Rendering::Index, 3>> normalIndices;
    };

    struct OBJModelResources
    {
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;

        std::vector<Face> faces;
    };

    class OBJModelLoader
    {
    public:
        static std::expected<ModelData, ModelLoadingError> LoadModel(
            const std::filesystem::path& modelPath, bool doFlipUVs
        );

    private:
        static OBJModelResources ParseOBJFile(const std::filesystem::path& path);
        static ModelData ConvertToMeshData(const OBJModelResources& modelResources, bool doFlipUVs);

    private:
        static void AppendVertices(OBJModelResources& modelResources, std::istringstream& currentLineStream);
        static void AppendTextures(OBJModelResources& modelResources, std::istringstream& currentLineStream);
        static void AppendNormals(OBJModelResources& modelResources, std::istringstream& currentLineStream);

    private:
        static void AppendFaces(OBJModelResources& modelResources, std::istringstream& currentLineStream);
    };
}