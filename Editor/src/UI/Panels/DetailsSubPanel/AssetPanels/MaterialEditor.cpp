//
// Created by Oskar.Norberg on 2026-02-09.
//

#include "UI/Panels/DetailsSubPanel/AssetPanels/MaterialEditor.h"

#include "Assets/AssetLoader.h"
#include "Assets/RuntimeAssetRegistry/RuntimeAssetRegistry.h"
#include "Logging/Logger.h"
#include "UI/AssetDragAndDrop.h"

namespace RNGOEngine::Editor
{
    void MaterialEditor::RenderSubPanel(UIContext& context)
    {
        const auto materialOpt = GetHandleFromSelection(context.selectionManager);

        if (!materialOpt)
        {
            ImGui::Text("No material selected.");
            return;
        }

        const auto& materialHandle = materialOpt.value();
        auto* materialAsset = GetMaterialAsset(materialHandle);

        if (!materialAsset)
        {
            ImGui::Text("Is your material loaded?");
            return;
        }

        ImGui::Text("Shader: %llu", materialAsset->GetShaderHandle().UUID.GetValue());

        // Textures
        for (auto& parameter : materialAsset->GetParameters().GetAll())
        {
            ImGui::Text("%s", parameter.Name.c_str());

            if (std::holds_alternative<AssetHandling::MaterialTextureSpecification>(parameter.Parameter))
            {
                const auto& textureSpec = std::get<AssetHandling::MaterialTextureSpecification>(parameter.Parameter);
                ImGui::Text("%s: %llu", parameter.Name.c_str(), textureSpec.TextureHandle.UUID.GetValue());

                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(AssetDragAndDropName))
                    {
                        IM_ASSERT(payload->DataSize == sizeof(AssetDragAndDropPayload));

                        const auto& dragAndDropPayload =
                            *static_cast<const AssetDragAndDropPayload*>(payload->Data);

                        if (dragAndDropPayload.Handle.Type != AssetHandling::AssetType::Texture)
                        {
                            RNGO_LOG(
                                Core::LogLevel::Warning, "Cannot load asset {} as a texture.",
                                dragAndDropPayload.Handle.UUID.GetValue()
                            );
                        }
                        else
                        {
                            AssetHandling::AssetLoader::GetInstance().Load(dragAndDropPayload.Handle);
                            parameter.Parameter = AssetHandling::MaterialTextureSpecification{
                                .TextureHandle = AssetHandling::TextureHandle{dragAndDropPayload.Handle.UUID},
                                .Slot = textureSpec.Slot,
                            };
                        }
                    }

                    ImGui::EndDragDropTarget();
                }
            }
        }
    }

    std::optional<AssetHandling::AssetHandle> MaterialEditor::GetHandleFromSelection(
        const UISelectionManager& selectionManager
    )
    {
        const auto currentSelection = selectionManager.GetCurrentSelection();
        // TODO: Make a helper method to check if current selection is of type.
        if (!std::holds_alternative<AssetSelection>(currentSelection))
        {
            return std::nullopt;
        }

        const auto& assetSelection = std::get<AssetSelection>(currentSelection);
        if (assetSelection.AssetHandle.Type != AssetHandling::AssetType::Material)
        {
            return std::nullopt;
        }

        if (!Utilities::IsValidUUID(assetSelection.AssetHandle.UUID))
        {
            return std::nullopt;
        }

        return assetSelection.AssetHandle;
    }

    AssetHandling::MaterialAsset* MaterialEditor::GetMaterialAsset(
        const AssetHandling::AssetHandle& materialHandle
    )
    {
        auto& registry = AssetHandling::RuntimeAssetRegistry::GetInstance();
        const auto assetOpt = registry.TryGet<AssetHandling::MaterialAsset>(materialHandle);

        if (!assetOpt)
        {
            return nullptr;
        }

        return &assetOpt.value().get();
    }
}