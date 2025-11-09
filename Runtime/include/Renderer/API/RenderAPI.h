//
// Created by ringo on 2025-10-05.
//

#pragma once

#include "RenderPass/RenderContext.h"
#include "RenderPass/RenderPass.h"
#include "Renderer/DrawQueue.h"

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
        void RenderToScreen();

    public:
        template<typename T, typename... Args>
        T& RegisterPass(Args&&... args)
        {
            m_passes.push_back(std::make_unique<T>(std::forward<Args>(args)...));

            auto passSpecification = m_passes.back()->GetRenderTargetSpecification();
            for (auto& attachment : passSpecification.Attachments)
            {
                if (attachment.Size.SizeType == Resources::AttachmentSizeType::PercentOfScreen)
                {
                    attachment.Size.width = static_cast<unsigned int>(
                        (static_cast<float>(attachment.Size.width) / 100.0f) * static_cast<float>(m_width));
                    attachment.Size.height = static_cast<unsigned int>(
                        (static_cast<float>(attachment.Size.height) / 100.0f) * static_cast<float>(m_height));
                }
            }

            const auto key = Resources::ResourceManager::GetInstance().GetRenderTargetManager().CreateFrameTarget(
                passSpecification);
            context.renderPassResources.RegisterRenderTarget(passSpecification.Name, key);

            return static_cast<T&>(*m_passes.back());
        }

    public:
        /// 
        /// @param eventQueue Engine event queue to send / listen to events from.
        /// @return True if there are more events to process, false otherwise.
        ///
        bool ListenSendEvents(Events::EventQueue& eventQueue);

    private:
        IRenderer& m_renderer;

    private:
        RenderContext context;
        std::vector<std::unique_ptr<RenderPass>> m_passes;

    private:
        int m_width, m_height;
    };
}