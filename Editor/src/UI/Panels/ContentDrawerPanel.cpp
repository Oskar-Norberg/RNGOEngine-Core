//
// Created by Oskar.Norberg on 2025-12-12.
//

#include "UI/Panels/ContentDrawerPanel.h"

#include "UI/AssetDragAndDrop.h"
#include "magic_enum/magic_enum.hpp"

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

        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_directory())
            {
                folder.Folders.push_back(entry.path());
            }
            else
            {
                // TODO: Store asset type in the metadata and load that instead of just listing all files in path.

                // TODO: EXTREMELY TEMPORARY HACK
                AssetHandling::AssetType type = AssetHandling::AssetType::None;
                const auto extension = entry.path().extension().string();
                if (extension == ".obj" || extension == ".fbx" || extension == ".gltf")
                {
                    type = AssetHandling::AssetType::Model;
                }
                folder.Assets.emplace_back(type, entry.path());
            }
        }

        return folder;
    }

    void ContentDrawerPanel::DrawHeader(UIContext& context)
    {
        if (ImGui::Button("Refresh"))
        {
            SetDeferredPath(m_currentPath);
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

        if (ImGui::BeginDragDropSource(src_flags))
        {
            if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
            {
                ImGui::Text("Moving");
            }

            assetDragAndDropPayload = {type, path};
            ImGui::SetDragDropPayload(
                AssetDragAndDropName, &assetDragAndDropPayload, sizeof(AssetDragAndDropPayload)
            );
            ImGui::EndDragDropSource();
        }
    }
}