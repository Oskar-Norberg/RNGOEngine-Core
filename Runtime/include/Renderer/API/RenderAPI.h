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

            const auto passSpecification = m_passes.back()->GetRenderTargetSpecification();

            // TODO: Should you not be able to create an empty RenderTarget?
            if (!passSpecification.Attachments.empty())
            {
                auto& targetManager = Resources::ResourceManager::GetInstance().GetRenderTargetManager();
                const auto key = targetManager.CreateRenderTarget();
                m_context.renderPassResources.RegisterRenderTarget(passSpecification.Name, key);

                for (const auto& attachment : passSpecification.Attachments)
                {
                    const auto attachmentKey = targetManager.CreateFrameBufferAttachment(
                        key, attachment.Format, attachment.AttachmentPoint, attachment.Size.width,
                        attachment.Size.height
                    );

                    // Add to resource mapper
                    if (std::holds_alternative<Texture2DProperties>(attachment.Format))
                    {
                        m_context.renderPassResources.RegisterTextureAttachment(
                            attachment.Name, attachmentKey
                        );
                    }
                    else if (std::holds_alternative<RenderBufferFormat>(attachment.Format))
                    {
                        m_context.renderPassResources.RegisterRenderBufferAttachment(
                            attachment.Name, attachmentKey
                        );
                    }
                }
            }

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
        RenderContext m_context;
        std::vector<std::unique_ptr<RenderPass>> m_passes;

    private:
        int m_width, m_height;

    private:
        void Render(
            int width, int height, std::optional<std::reference_wrapper<Resources::RenderTarget>> target
        );
    };
}