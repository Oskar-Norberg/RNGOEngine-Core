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
    // Higher level abstraction of FrameBuffers, Attachments and RBOs.
    struct FrameBufferAttachment
    {
        std::variant<TextureID, RenderBufferID> Resource;
        TextureFormat Format;
        int width, height;
        FrameBufferAttachmentPoint AttachmentPoint;


        bool DoClearColor = false;
        bool DoClearDepth = false;
        glm::vec4 ClearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    };

    struct RenderTarget
    {
        std::string DebugName = "Unnamed Render Target";
        FrameBufferID FrameBuffer = INVALID_FRAMEBUFFER_ID;

        std::vector<FrameBufferAttachment> Attachments;
    };
}