//
// Created by ringo on 2025-11-08.
//

#pragma once

#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "Renderer/RenderID.h"
#include "glm/vec4.hpp"

namespace RNGOEngine::Resources
{
    enum class AttachmentSizeType
    {
        Absolute, // Fixed size
        PercentOfScreen, // Size relative to current screen
    };

    struct AttachmentSize
    {
        AttachmentSizeType SizeType = AttachmentSizeType::Absolute;
        unsigned int width, height;
    };

    enum class AttachmentType { Texture, RenderBuffer };

    // Higher level abstraction of FrameBuffers, Attachments and RBOs.
    struct FrameBufferAttachment
    {
        std::string AttachmentName = "Unnamed Attachment";
        // TODO: Make TextureID and RenderBufferID strongly typed to make use of variant.
        AttachmentType Type = AttachmentType::Texture;
        unsigned int ID = 0; // TextureID or RenderBufferID
        Core::Renderer::TextureFormat Format;
        Core::Renderer::FrameBufferAttachmentPoint AttachmentPoint;
        AttachmentSize Size;
    };

    struct RenderTarget
    {
        std::string TargetName = "Unnamed Render Target";
        std::optional<Core::Renderer::FrameBufferID> FrameBuffer;

        std::vector<FrameBufferAttachment> Attachments;
    };


    // Specifications for creation of Targets/Attachments
    enum FrameBufferAttachmentSpecificationType
    {
        Texture,
        RenderBuffer,
    };

    struct FrameBufferAttachmentSpecification
    {
        std::string Name;
        FrameBufferAttachmentSpecificationType Type;
        Core::Renderer::TextureFormat Format;
        // TODO: This does not work for RenderBuffers
        Core::Renderer::TextureFiltering minifyingFilter;
        Core::Renderer::TextureFiltering magnifyingFilter;
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