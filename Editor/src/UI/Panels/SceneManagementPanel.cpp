//
// Created by Oskar.Norberg on 2026-01-26.
//

#include "UI/Panels/SceneManagementPanel.h"

#include <yaml-cpp/yaml.h>

#include <string>

#include "Logging/Logger.h"
#include "Scene/SceneManager/SceneManager.h"
#include "Utilities/AssetCache/AssetCache.h"
#include "Utilities/IO/SimpleFileReader/SimpleFileReader.h"

namespace RNGOEngine::Editor
{
    static std::string GetSceneFilePath(const std::string_view sceneName)
    {
        return std::string(sceneName) + ".rngoscene";
    }

    void SceneManagementPanel::Render(UIContext& context)
    {
        IDockablePanel::Render(context);

        static char sceneNameBuffer[256] = "";
        ImGui::InputText("Scene Name", sceneNameBuffer, sizeof(sceneNameBuffer));

        if (ImGui::Button("Load Scene"))
        {
            const auto path = GetSceneFilePath(sceneNameBuffer);

            if (!Utilities::IO::FileExists(path))
            {
                RNGO_LOG(Core::LogLevel::Warning, "Scene file '{}' does not exist.", path);
                return;
            }

            auto yaml = YAML::LoadFile(path);
            context.sceneManager->GetCurrentScene()->Deserialize(yaml);
        }

        if (ImGui::Button("Save Scene"))
        {
            // TODO: Currently just outputs to CWD, integrate to asset system? or just open a file dialogue?
            const auto path = GetSceneFilePath(sceneNameBuffer);
            YAML::Emitter emitter;
            context.sceneManager->GetCurrentScene()->Serialize(emitter);
            Utilities::IO::WriteToFile(emitter.c_str(), path);
        }
    }
}