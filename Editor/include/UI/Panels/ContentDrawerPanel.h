//
// Created by Oskar.Norberg on 2025-12-12.
//

#pragma once

#include <filesystem>

#include "Assets/Asset.h"
#include "UI/IDockablePanel.h"

namespace RNGOEngine::Editor
{
    struct SerializedAsset
    {
        AssetHandling::AssetType Type;
        std::filesystem::path Path;
    };

    struct Folder
    {
        std::vector<std::filesystem::path> Folders;
        std::vector<SerializedAsset> Assets;
    };

    class ContentDrawerPanel : public IDockablePanel
    {
    public:
        ContentDrawerPanel();
        void Render(UIContext& context) override;

        std::string_view GetPanelName() const override
        {
            return "Content Drawer";
        }

    private:
        std::optional<std::filesystem::path> m_queuedSelectionPath;
        std::filesystem::path m_currentPath;
        Folder m_currentFolder;

    private:
        void DrawHeader(UIContext& context);
        void DrawContent(UIContext& context);

    private:
        void CheckDeferredPath();
        void SetDeferredPath(const std::filesystem::path& path);

    private:
        void DrawFolder(UIContext& context, std::string_view label, const std::filesystem::path& path);
        void DrawAsset(
            UIContext& context, std::string_view label, AssetHandling::AssetType type,
            const std::filesystem::path& path
        );

    private:
        void RefreshCurrentFolder();

    private:
        static Folder LoadFolder(const std::filesystem::path& path);
        static std::unordered_map<std::filesystem::path, AssetHandling::AssetType> ScanMetadata(
            const std::filesystem::path& path, Folder& outFolder
        );
        static void ScanAssets(
            const std::filesystem::path& path,
            const std::unordered_map<std::filesystem::path, AssetHandling::AssetType>& metadataMap,
            Folder& outFolder
        );
        static void ScanFolders(
            const std::filesystem::path& path,
            Folder& outFolder
        );
        static void ScanUnimportedFiles(
            const std::filesystem::path& path,
            const std::unordered_map<std::filesystem::path, AssetHandling::AssetType>& metadataMap,
            Folder& outFolder
        );
    };
}