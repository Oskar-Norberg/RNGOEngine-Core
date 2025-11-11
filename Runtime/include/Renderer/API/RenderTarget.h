//
// Created by ringo on 2025-11-08.
//

#pragma once

#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "Renderer/RenderID.h"

namespace RNGOEngine::Resources
{
    enum class AttachmentSizeType
    {
        Absolute,         // Fixed size
        PercentOfScreen,  // Size relative to current screen
    };

    struct AttachmentSize
    {
        AttachmentSizeType SizeType = AttachmentSizeType::Absolute;
        unsigned int width, height;
    };

    struct FrameBufferAttachmentSpecification
    {
        std::string Name;
        std::variant<Core::Renderer::Texture2DProperties, Core::Renderer::RenderBufferFormat> Format;
        Core::Renderer::FrameBufferAttachmentPoint AttachmentPoint;

        AttachmentSize Size;
    };

    // Specifications for creation of runtime RenderTargets.
    struct RenderTargetSpecification
    {
        std::string Name = "Unnamed Render Target";
        bool CreateFrameBuffer = true;
        // TODO: Having inputs doesn't belong to the RenderTarget itself. That is a RenderPass concept.
        std::vector<std::string> InputNames;
        std::vector<FrameBufferAttachmentSpecification> Attachments;
    };
}