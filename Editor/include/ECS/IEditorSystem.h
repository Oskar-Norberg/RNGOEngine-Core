//
// Created by Oskar.Norberg on 2025-11-17.
//

#pragma once

#include "EditorSystemContext.h"
#include "ECS/Systems/ISystem.h"

namespace RNGOEngine::Editor
{
    class IEditorSystem : public Systems::ISystem<EditorSystemContext>
    {
    };
}