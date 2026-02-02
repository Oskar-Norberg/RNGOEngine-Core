//
// Created by Oskar.Norberg on 2026-01-26.
//

#pragma once
#include <functional>
#include <memory>
#include <span>

#include "Utilities/Concepts/Concepts.h"
#include "IDockablePanel.h"

namespace RNGOEngine::Editor::UI
{
    using PanelFactory = std::function<std::unique_ptr<IDockablePanel>()>;

    inline std::vector<PanelFactory>& GetRegisteredPanelFactories()
    {
        static std::vector<PanelFactory> s_registeredPanelFactories;
        return s_registeredPanelFactories;
    }

    template<typename T>
        requires RNGOEngine::Concepts::DefaultConstructible<T>
    struct UIPanelInitializer
    {
        UIPanelInitializer()
        {
            GetRegisteredPanelFactories().push_back(
                []()
                {
                    return std::make_unique<T>();
                }
            );
        }
    };

#define EDITOR_PANEL(PanelType) \
    inline RNGOEngine::Editor::UI::UIPanelInitializer<PanelType> PanelType##Initializer;
}