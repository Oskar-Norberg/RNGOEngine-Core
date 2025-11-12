//
// Created by ringo on 2025-10-05.
//

#pragma once

#include "RenderPass/RenderContext.h"
#include "RenderPass/RenderPass.h"
#include "Renderer/DrawQueue.h"
#include "ResourceManager/ResourceManager.h"

namespace RNGOEngine
{
    namespace Events
    {
        class EventQueue;
    }
}

namespace RNGOEngine::Core::Renderer
{
    // TODO: There really needs to be explicit Begin/End frame function so we can clear out DrawQueues and draw imgui properly.
    class RenderAPI
    {
    public:
        explicit RenderAPI(IRenderer& renderer, int width, int height);

        void SubmitDrawQueue(DrawQueue&& drawQueue);
        // TODO: I kind of dislike this not being const.
        // TODO: Pass deltaTime / frame info? Pass in Target FrameBuffer and its parameters. (Wrap into a FrameBuffer struct)
        void RenderToScreen(int width, int height);
        void RenderToTarget(
            int width, int height, Containers::GenerationalKey<Resources::RenderTarget> targetKey
        );

    public:
        template<typename T, typename... Args>
        T& RegisterPass(Args&&... args)
        {
            m_passes.push_back(std::make_unique<T>(std::forward<Args>(args)...));

            // If target defines any attachments.
            const auto passSpecification = m_passes.back()->GetRenderTargetSpecification();
            if (!passSpecification.Attachments.empty())
            {
                CreateRenderTarget(passSpecification, m_width, m_height);
            }

            return static_cast<T&>(*m_passes.back());
        }

    public:
        ///
        /// @param eventQueue Engine event queue to send / listen to events from.
        /// @return True if there are more events to process, false otherwise.
        ///
        bool ListenSendEvents(Events::EventQueue& eventQueue);

    public:
        Containers::GenerationalKey<Resources::RenderTarget> CreateRenderTarget(
            const Resources::RenderTargetSpecification& specification, int width, int height);

    private:
        IRenderer& m_renderer;

    private:
        RenderContext m_context;
        std::vector<std::unique_ptr<RenderPass>> m_passes;
        std::unordered_map<Resources::RenderTargetSpecification, Containers::GenerationalKey<
                               Resources::RenderTarget>> m_managedTargets;

    private:
        int m_width, m_height;

    private:
        void Render(
            int width, int height, std::optional<std::reference_wrapper<Resources::RenderTarget>> target
        );
    };
}