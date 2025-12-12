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
        void DrawFolder(UIContext& context, std::string_view label, const std::filesystem::path& path);
        void DrawAsset(
            UIContext& context, std::string_view label, AssetHandling::AssetType type,
            const std::filesystem::path& path
        );

    private:
        static Folder LoadFolder(const std::filesystem::path& path);
    };
}