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

    void SceneManagementPanel::Render(UIContext& context)
    {
        IDockablePanel::Render(context);

        static char sceneNameBuffer[256] = "";
        ImGui::InputText("Scene Name", sceneNameBuffer, sizeof(sceneNameBuffer));

        if (ImGui::Button("Load Scene"))
        {
            const auto path = std::string(sceneNameBuffer);

            if (!Utilities::IO::FileExists(path))
            {
                RNGO_LOG(Core::LogLevel::Warning, "Scene file '{}' does not exist.", path);
                return;
            }

            auto yaml = YAML::LoadFile(std::string(sceneNameBuffer));
            context.sceneManager->GetCurrentScene()->Deserialize(yaml);
        }

        if (ImGui::Button("Save Scene"))
        {
            const auto path = std::string(sceneNameBuffer);
            YAML::Emitter emitter;
            context.sceneManager->GetCurrentScene()->Serialize(emitter);
            Utilities::IO::WriteToFile(path, emitter.c_str());
        }
    }
}