//
// Created by ringo on 2025-11-12.
//

#pragma once

#include "ECS/IEditorSystem.h"

namespace RNGOEngine::Editor
{
    class FreeFlyCameraSystem : public IEditorSystem
    {
    public:
        ~FreeFlyCameraSystem() override
        {
        }
        void Update(Core::World& world, EditorSystemContext& context) override;

    private:
        static constexpr auto CAMERA_SENSITIVITY = 100.0f;
        static constexpr auto CAMERA_SPEED = 10.0f;
        static constexpr auto CAMERA_BOOST_MULTIPLIER = 2.0f;
    };
}