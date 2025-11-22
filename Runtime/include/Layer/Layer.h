//
// Created by ringo on 2025-11-22.
//

#pragma once

namespace RNGOEngine::Core
{
    class Layer
    {
    public:
        Layer() = default;
        virtual ~Layer() = default;

    public:
        virtual void OnAttach(){}
        virtual void OnDetach(){}

    public:
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnRender() {}
        // TODO: OnEvent handling
    };
}