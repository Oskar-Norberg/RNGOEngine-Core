//
// Created by ringo on 2025-11-08.
//

#pragma once

#include <string>
#include <variant>
#include <vector>

#include "Renderer/RenderID.h"
#include "glm/vec4.hpp"

namespace RNGOEngine::Core::Renderer
{
    enum class AttachmentSizeType
    {
        Absolute, // Fixed size
        PercentOfScreen, // Size relative to current screen
    };

    struct AttachmentSize
    {
        AttachmentSizeType SizeType = AttachmentSizeType::Absolute;
        int width, height;
    };

    // Higher level abstraction of FrameBuffers, Attachments and RBOs.
    struct FrameBufferAttachment
    {
        std::variant<TextureID, RenderBufferID> Resource;
        TextureFormat Format;
        FrameBufferAttachmentPoint AttachmentPoint;
        AttachmentSize Size;

        bool DoClearColor = false;
        bool DoClearDepth = false;
        glm::vec4 ClearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    };

    struct RenderTarget
    {
        std::string TargetName = "Unnamed Render Target";
        FrameBufferID FrameBuffer = INVALID_FRAMEBUFFER_ID;

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
        TextureFormat Format;
        FrameBufferAttachmentPoint AttachmentPoint;

        AttachmentSize Size;

        bool DoClearColor = false;
        bool DoClearDepth = false;
        glm::vec4 ClearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    };

    // Specifications for creation of runtime RenderTargets.
    struct RenderTargetSpecification
    {
        std::string Name = "Unnamed Render Target";
        bool CreateFrameBuffer = true;
        std::vector<std::string> InputNames;
        std::vector<FrameBufferAttachmentSpecification> Attachments;
    };
}