//
// Created by Oskar.Norberg on 2025-12-12.
//

#include "UI/Panels/ContentDrawerPanel.h"

#include "magic_enum/magic_enum.hpp"
#include "spdlog/fmt/bundled/compile.h"

namespace RNGOEngine::Editor
{
    ContentDrawerPanel::ContentDrawerPanel()
        : m_currentPath(std::filesystem::current_path()), m_currentFolder(LoadFolder(m_currentPath))
    {
    }

    void ContentDrawerPanel::Render(UIContext& context)
    {
        IDockablePanel::Render(context);

        if (m_queuedSelectionPath)
        {
            m_currentPath = m_queuedSelectionPath.value();
            m_currentFolder = LoadFolder(m_currentPath);
        }

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

                ImGui::Text(
                    "[%s] %s", magic_enum::enum_name(asset.Type).data(), asset.Path.filename().string().data()
                );
            }

            ImGui::EndTable();
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
                folder.Assets.emplace_back(AssetHandling::AssetType::None, entry.path());
            }
        }

        return folder;
    }

    void ContentDrawerPanel::DrawFolder(
        UIContext& context, std::string_view label, const std::filesystem::path& path
    )
    {
        if (ImGui::Selectable(label.data(), false, ImGuiSelectableFlags_AllowDoubleClick))
        {
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                m_queuedSelectionPath = path;
            }
        }
    }
}