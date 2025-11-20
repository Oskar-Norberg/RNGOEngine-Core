//
// Created by ringo on 2025-11-12.
//

#include "ECS/Systems/FreeFlyCameraSystem.h"

#include "Components/Components.h"
#include "Data/KeyCodes.h"
#include "Data/MouseCodes.h"
#include "Systems/SystemContext.h"
#include "World/World.h"

namespace RNGOEngine::Editor
{

    void FreeFlyCameraSystem::Update(Core::World& world, EditorSystemContext& context)
    {
        IEditorSystem::Update(world, context);

        auto& inputManager = *context.inputManager;

        // Not sure this is the best place for this, but works for now.
        if (inputManager.WasMouseButtonPressedThisFrame(Data::MouseCodes::RNGO_MOUSE_BUTTON_RIGHT))
        {
            inputManager.SetMouseMode(Data::Mouse::MouseMode::Locked);
        }
        else if (inputManager.WasMouseButtonReleasedThisFrame(Data::MouseCodes::RNGO_MOUSE_BUTTON_RIGHT))
        {
            inputManager.SetMouseMode(Data::Mouse::MouseMode::Normal);
        }

        if (!inputManager.IsMouseButtonDown(Data::MouseCodes::RNGO_MOUSE_BUTTON_RIGHT))
        {
            return;
        }

        const auto cameraSpeed = inputManager.IsKeyDown(Data::KeyCodes::RNGO_KEY_LEFT_SHIFT)
                                     ? CAMERA_SPEED * CAMERA_BOOST_MULTIPLIER
                                     : CAMERA_SPEED;

        const auto view = world.GetRegistry().view<Components::Camera, Components::Transform>();
        for (const auto [entity, camera, transform] : view.each())
        {
            glm::vec3 upward = glm::normalize(transform.Rotation * glm::vec3(0.0f, 1.0f, 0.0f));
            glm::vec3 forward = glm::normalize(transform.Rotation * glm::vec3(0.0f, 0.0f, -1.0f));
            glm::vec3 right = glm::normalize(transform.Rotation * glm::vec3(1.0f, 0.0f, 0.0f));

            // Horizontal Plane Movement
            {
                glm::vec3 moveDirection(0.0f);

                if (inputManager.IsKeyDown(Data::KeyCodes::RNGO_KEY_W))
                {
                    moveDirection += forward;
                }
                if (inputManager.IsKeyDown(Data::KeyCodes::RNGO_KEY_A))
                {
                    moveDirection -= right;
                }
                if (inputManager.IsKeyDown(Data::KeyCodes::RNGO_KEY_S))
                {
                    moveDirection -= forward;
                }
                if (inputManager.IsKeyDown(Data::KeyCodes::RNGO_KEY_D))
                {
                    moveDirection += right;
                }

                if (moveDirection != glm::vec3(0.0f))
                {
                    moveDirection = glm::normalize(moveDirection);

                    transform.Position += moveDirection * cameraSpeed * context.deltaTime;
                }
            }

            // Vertical Movement
            {
                if (inputManager.IsKeyDown(Data::KeyCodes::RNGO_KEY_Q))
                {
                    transform.Position -= upward * cameraSpeed * context.deltaTime;
                }
                if (inputManager.IsKeyDown(Data::KeyCodes::RNGO_KEY_E))
                {
                    transform.Position += upward * cameraSpeed * context.deltaTime;
                }
            }

            // Mouse Look
            {
                const auto mouseDelta = inputManager.GetMouseDelta();
                if (mouseDelta.x != 0.0 || mouseDelta.y != 0.0)
                {
                    float yaw = glm::radians(static_cast<float>(-mouseDelta.x) * CAMERA_SENSITIVITY);
                    float pitch = glm::radians(static_cast<float>(-mouseDelta.y) * CAMERA_SENSITIVITY);

                    glm::quat quatYaw = glm::angleAxis(yaw * context.deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
                    glm::quat quatPitch = glm::angleAxis(pitch * context.deltaTime, glm::normalize(right));

                    transform.Rotation = glm::normalize(quatYaw * quatPitch * transform.Rotation);
                }
            }
        }
    }
}