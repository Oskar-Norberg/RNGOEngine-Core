//
// Created by Oskar.Norberg on 2025-12-12.
//

#pragma once

#include <filesystem>

#include "Assets/Asset.h"
#include "Assets/AssetDatabase/AssetDatabase.h"
#include "UI/IDockablePanel.h"
#include "UI/PanelInitializer.h"
#include "Utilities/IO/SimpleFileReader/SimpleFileReader.h"

namespace RNGOEngine::Editor
{
    struct UnregisteredAsset
    {
        std::filesystem::path Path;
    };

    struct RegisteredAsset
    {
        AssetHandling::AssetHandle Handle;
        std::filesystem::path Path;
    };
    using AssetEntry = std::variant<UnregisteredAsset, RegisteredAsset>;

    struct Folder
    {
        std::vector<std::filesystem::path> SubFolders;
        std::vector<AssetEntry> Assets;
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
        struct CurrentFolder
        {
            std::filesystem::path Path;
            Folder Content;
        } m_currentFolder;

        std::optional<std::filesystem::path> m_deferredPathOpt;

    private:
        void SetDeferredPath(const std::filesystem::path& path);
        void LoadDeferredPathIfAny();

    private:
        void RenderFolderView(UIContext& context);
        void DrawHeader(UIContext& context);
        void DrawBody(UIContext& context);

        void RenderSubfolders(UIContext& context);
        void RenderAssets(UIContext& context);

    private:
        void RenderFolderEntry(
            const std::filesystem::path& folderPath, std::optional<std::string> customLabel,
            UIContext& context
        );
        void RenderAssetEntry(const AssetEntry& assetEntry, UIContext& context);
    };
    EDITOR_PANEL(ContentDrawerPanel);

    // TODO: Should statics go in the source?
    static Folder GetFolderContent(const std::filesystem::path& folderPath)
    {
        Folder folderContent;

        // Add Folders
        for (const auto& entry : std::filesystem::directory_iterator(folderPath))
        {
            if (!entry.is_directory())
            {
                continue;
            }

            folderContent.SubFolders.emplace_back(entry.path());
        }

        // Find registered Assets
        for (const auto& entry : std::filesystem::directory_iterator(folderPath))
        {
            // TODO: Ideally, the file extension should be easily changeable.
            if (entry.is_directory() || entry.path().extension() == ".meta")
            {
                continue;
            }

            const auto& assetDB = AssetHandling::AssetDatabase::GetInstance();

            if (assetDB.IsRegistered(entry.path()))
            {
                const auto assetHandle = assetDB.GetAssetHandle(entry.path());
                const RegisteredAsset registeredAsset{.Handle = assetHandle, .Path = entry.path()};
                folderContent.Assets.emplace_back(registeredAsset);
            }
            else
            {
                const UnregisteredAsset unregisteredAsset{.Path = entry.path()};
                folderContent.Assets.emplace_back(unregisteredAsset);
            }
        }

        return folderContent;
    }
}