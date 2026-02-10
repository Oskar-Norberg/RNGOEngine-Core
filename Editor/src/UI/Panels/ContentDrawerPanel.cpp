//
// Created by Oskar.Norberg on 2025-12-12.
//

#include "UI/Panels/ContentDrawerPanel.h"

#include "Assets/AssetLoader.h"
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

        LoadDeferredPathIfAny();
        RenderFolderView(context);
    }

    void ContentDrawerPanel::SetDeferredPath(const std::filesystem::path& path)
    {
        m_deferredPathOpt = path;
    }

    void ContentDrawerPanel::LoadDeferredPathIfAny()
    {
        if (!m_deferredPathOpt)
        {
            return;
        }

        const auto& folderToLoad = m_deferredPathOpt.value();
        m_currentFolder = CurrentFolder{.Path = folderToLoad, .Content = GetFolderContent(folderToLoad)};

        m_deferredPathOpt.reset();
    }

    void ContentDrawerPanel::RenderFolderView(UIContext& context)
    {
        DrawHeader(context);
        DrawBody(context);
    }

    void ContentDrawerPanel::DrawHeader(UIContext& context)
    {
        if (ImGui::Button("Refresh"))
        {
            SetDeferredPath(m_currentFolder.Path);
        }
        ImGui::SameLine();
        if (ImGui::Button("Home"))
        {
            SetDeferredPath(std::filesystem::current_path());
        }
        ImGui::SameLine();
        ImGui::Text("%s", m_currentFolder.Path.string().data());
    }

    void ContentDrawerPanel::DrawBody(UIContext& context)
    {
        constexpr ImGuiTableFlags tableFlags = ImGuiTableFlags_RowBg;
        if (ImGui::BeginTable("Folder", 1, tableFlags))
        {
            // TODO: Render ".." entry.
            RenderSubfolders(context);
            RenderAssets(context);

            ImGui::EndTable();
        }
    }

    void ContentDrawerPanel::RenderSubfolders(UIContext& context)
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        RenderFolderEntry(m_currentFolder.Path.parent_path(), "..", context);

        for (const auto& subfolderPath : m_currentFolder.Content.SubFolders)
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            // TODO: Just do default args :sob:
            RenderFolderEntry(subfolderPath, std::nullopt, context);
        }
    }

    void ContentDrawerPanel::RenderAssets(UIContext& context)
    {
        for (const auto& assetEntry : m_currentFolder.Content.Assets)
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            RenderAssetEntry(assetEntry, context);
        }
    }
    void ContentDrawerPanel::RenderFolderEntry(
        const std::filesystem::path& folderPath, std::optional<std::string> customLabel, UIContext& context
    )
    {
        const std::string folderLabel = "[Folder] " + customLabel.value_or(folderPath.filename().string());

        if (ImGui::Selectable(folderLabel.data(), false, ImGuiSelectableFlags_AllowDoubleClick))
        {
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                SetDeferredPath(folderPath);
            }
        }
    }

    // TODO: Too long.
    void ContentDrawerPanel::RenderAssetEntry(const AssetEntry& assetEntry, UIContext& context)
    {
        std::optional<AssetHandling::AssetHandle> assetHandleOpt;
        auto path = std::filesystem::path{};

        if (std::holds_alternative<RegisteredAsset>(assetEntry))
        {
            const auto& registeredAsset = std::get<RegisteredAsset>(assetEntry);
            assetHandleOpt =
                AssetHandling::AssetHandle{registeredAsset.Handle.UUID, registeredAsset.Handle.Type};
            path = registeredAsset.Path;
        }
        else if (std::holds_alternative<UnregisteredAsset>(assetEntry))
        {
            const auto& unregisteredAsset = std::get<UnregisteredAsset>(assetEntry);
            path = unregisteredAsset.Path;
        }

        const auto labelWithType = assetHandleOpt.has_value()
                                       ? "[" + std::string(magic_enum::enum_name(assetHandleOpt->Type)) +
                                             "] " + path.filename().string()
                                       : "[None] " + path.filename().string();

        // const auto labelWithType =
        //     "[" + std::string(magic_enum::enum_name(assetType)) + "] " + path.filename().string();

        if (ImGui::Selectable(labelWithType.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
        {
            if (assetHandleOpt.has_value())
            {
                context.selectionManager.SelectAsset(assetHandleOpt.value());
            }
            else
            {
                context.selectionManager.Deselect();
            }
        }

        if (ImGui::BeginPopupContextItem())
        {
            ImGui::Text("AssetContextMenu");

            if (ImGui::MenuItem("Import"))
            {
                if (AssetHandling::AssetLoader::GetInstance().TryImport(path))
                {
                    SetDeferredPath(m_currentFolder.Path);
                }

                ImGui::CloseCurrentPopup();
            }

            // Debug only
            if (ImGui::MenuItem("Force Load"))
            {
                const auto loadedAssetOpt = AssetHandling::AssetLoader::GetInstance().TryImport(path);
                if (loadedAssetOpt)
                {
                    SetDeferredPath(m_currentFolder.Path);
                    AssetHandling::AssetLoader::GetInstance().Load(loadedAssetOpt.value());
                }

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        static AssetDragAndDropPayload assetDragAndDropPayload;
        constexpr ImGuiDragDropFlags src_flags =
            ImGuiDragDropFlags_SourceNoDisableHover | ImGuiDragDropFlags_SourceNoHoldToOpenOthers;

        if (assetHandleOpt.has_value())
        {
            if (ImGui::BeginDragDropSource(src_flags))
            {
                if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
                {
                    ImGui::Text("Moving");
                }

                assetDragAndDropPayload.Handle = assetHandleOpt.value();

                ImGui::SetDragDropPayload(
                    AssetDragAndDropName, &assetDragAndDropPayload, sizeof(AssetDragAndDropPayload)
                );

                ImGui::EndDragDropSource();
            }
        }
    }
}
