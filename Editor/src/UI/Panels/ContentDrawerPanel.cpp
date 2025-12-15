//
// Created by Oskar.Norberg on 2025-12-12.
//

#include "UI/Panels/ContentDrawerPanel.h"

#include <unordered_set>

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "UI/AssetDragAndDrop.h"
#include "magic_enum/magic_enum.hpp"
#include "yaml-cpp/node/node.h"
#include "yaml-cpp/node/parse.h"

namespace RNGOEngine::Editor
{
    ContentDrawerPanel::ContentDrawerPanel()
    {
        SetDeferredPath(std::filesystem::current_path());
    }

    void ContentDrawerPanel::Render(UIContext& context)
    {
        IDockablePanel::Render(context);

        // Preparation
        CheckDeferredPath();

        // Rendering
        DrawHeader(context);
        ImGui::Separator();
        {
            ImGui::BeginChild("ContentDrawerPanelContent");
            DrawContent(context);
            ImGui::EndChild();
        }
    }

    Folder ContentDrawerPanel::LoadFolder(const std::filesystem::path& path)
    {
        Folder folder;

        const auto metadataMap = ScanMetadata(path, folder);

        ScanAssets(path, metadataMap, folder);
        ScanFolders(path, folder);
        ScanUnimportedFiles(path, metadataMap, folder);

        return folder;
    }

    std::unordered_map<std::filesystem::path, AssetHandling::AssetType> ContentDrawerPanel::ScanMetadata(
        const std::filesystem::path& path, Folder& outFolder
    )
    {
        std::unordered_map<std::filesystem::path, AssetHandling::AssetType> metadataMap;

        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_directory())
            {
                continue;
            }

            if (entry.path().extension() != ".meta")
            {
                continue;
            }

            YAML::Node node = YAML::LoadFile(entry.path().string());
            const auto typeOpt =
                magic_enum::enum_cast<AssetHandling::AssetType>(node["Type"].as<std::string>());

            if (!typeOpt)
            {
                // TODO: Log error? That's gonna spam console...
                continue;
            }

            const auto type = typeOpt.value();
            const auto assetPath = entry.path().parent_path() / entry.path().stem();
            metadataMap.emplace(assetPath, type);
        }

        return metadataMap;
    }

    void ContentDrawerPanel::ScanAssets(
        const std::filesystem::path& path,
        const std::unordered_map<std::filesystem::path, AssetHandling::AssetType>& metadataMap,
        Folder& outFolder
    )
    {
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_directory())
            {
                continue;
            }

            const auto it = metadataMap.find(entry.path());
            if (it != metadataMap.end())
            {
                outFolder.Assets.emplace_back(it->second, entry.path());
            }
        }
    }

    void ContentDrawerPanel::ScanFolders(const std::filesystem::path& path, Folder& outFolder)
    {
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (!entry.is_directory())
            {
                continue;
            }

            outFolder.Folders.emplace_back(entry.path());
        }
    }

    void ContentDrawerPanel::ScanUnimportedFiles(
        const std::filesystem::path& path,
        const std::unordered_map<std::filesystem::path, AssetHandling::AssetType>& metadataMap,
        Folder& outFolder
    )
    {
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_directory())
            {
                continue;
            }

            if (entry.path().extension() == ".meta")
            {
                continue;
            }

            if (metadataMap.contains(entry.path()))
            {
                continue;
            }

            outFolder.Assets.emplace_back(AssetHandling::AssetType::None, entry.path());
        }
    }

    void ContentDrawerPanel::DrawHeader(UIContext& context)
    {
        if (ImGui::Button("Refresh"))
        {
            SetDeferredPath(m_currentPath);
        }
        ImGui::SameLine();
        if (ImGui::Button("Home"))
        {
            SetDeferredPath(std::filesystem::current_path());
        }
    }

    void ContentDrawerPanel::DrawContent(UIContext& context)
    {
        constexpr ImGuiTableFlags tableFlags = ImGuiTableFlags_RowBg;
        if (ImGui::BeginTable("Folder", 1, tableFlags))
        {
            if (m_currentPath.has_parent_path())
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                std::string_view label = "..";
                DrawFolder(context, label, m_currentPath.parent_path());
            }

            for (const auto& folder : m_currentFolder.Folders)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                const auto folderName = folder.filename().string();
                const auto folderNameView = std::string_view(folderName);

                DrawFolder(context, folderNameView, folder);
            }

            for (const auto& asset : m_currentFolder.Assets)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                const auto label = asset.Path.filename().string();
                const auto labelView = std::string_view(label);
                DrawAsset(context, labelView, asset.Type, asset.Path);
            }

            ImGui::EndTable();
        }
    }

    void ContentDrawerPanel::CheckDeferredPath()
    {
        if (m_queuedSelectionPath)
        {
            m_currentPath = m_queuedSelectionPath.value();
            m_currentFolder = LoadFolder(m_currentPath);
            m_queuedSelectionPath.reset();
        }
    }

    void ContentDrawerPanel::SetDeferredPath(const std::filesystem::path& path)
    {
        m_queuedSelectionPath = path;
    }

    void ContentDrawerPanel::DrawFolder(
        UIContext& context, std::string_view label, const std::filesystem::path& path
    )
    {
        std::string folderLabel = "[Folder] " + std::string(label);
        if (ImGui::Selectable(folderLabel.data(), false, ImGuiSelectableFlags_AllowDoubleClick))
        {
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                SetDeferredPath(path);
            }
        }
    }

    void ContentDrawerPanel::DrawAsset(
        UIContext& context, std::string_view label, AssetHandling::AssetType type,
        const std::filesystem::path& path
    )
    {
        static AssetDragAndDropPayload assetDragAndDropPayload;

        const auto labelWithType =
            "[" + std::string(magic_enum::enum_name(type)) + "] " + path.filename().string();
        const auto labelWithTypeView = std::string_view(labelWithType);
        ImGui::Selectable(labelWithTypeView.data(), false, ImGuiSelectableFlags_AllowDoubleClick);

        constexpr ImGuiDragDropFlags src_flags =
            ImGuiDragDropFlags_SourceNoDisableHover | ImGuiDragDropFlags_SourceNoHoldToOpenOthers;

        if (AssetHandling::AssetDatabase::GetInstance().IsRegistered(path))
        {
            if (ImGui::BeginDragDropSource(src_flags))
            {
                if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
                {
                    ImGui::Text("Moving");
                }

                const auto handle = AssetHandling::AssetDatabase::GetInstance().GetAssetHandle(path);

                assetDragAndDropPayload = {type, handle};
                ImGui::SetDragDropPayload(
                    AssetDragAndDropName, &assetDragAndDropPayload, sizeof(AssetDragAndDropPayload)
                );

                ImGui::EndDragDropSource();
            }
        }
    }
}