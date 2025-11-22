//
// Created by ringo on 2025-11-22.
//

#pragma once

#include <memory>
#include <vector>

#include "Concepts/Concepts.h"
#include "Layer.h"

namespace RNGOEngine::Core
{
    // TODO: Layer suspensions?
    // TODO: Ability to order layers explicitly.
    class LayerStack
    {
    public:
        template<Concepts::DerivedFrom<Layer> T, typename... Args>
        T& PushLayer(Args&&... args);

        template<Concepts::DerivedFrom<Layer> T>
        bool PopLayer();

        //  Iterators
    public:
        auto First(){return m_layers.begin();}
        auto Last(){return m_layers.end();}

        const auto First() const{return m_layers.begin();}
        const auto Last() const{return m_layers.end();}

    private:
        std::vector<std::unique_ptr<Layer>> m_layers;
    };

#include "LayerStack.tpp"
}