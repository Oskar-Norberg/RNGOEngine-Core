//
// Created by ringo on 2025-11-08.
//

#pragma once

#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "Renderer/RenderID.h"
#include "Utilities/RNGOAsserts.h"
#include "Utilities/Hash/PairHash.h"

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
        std::vector<FrameBufferAttachmentSpecification> Attachments;

        bool operator==(const RenderTargetSpecification& other) const
        {
            if (Name != other.Name)
            {
                return false;
            }

            const size_t size = Attachments.size();
            if (size != other.Attachments.size())
            {
                return false;
            }

            for (size_t i = 0; i < size; ++i)
            {
                const auto& a = Attachments[i];
                const auto& b = other.Attachments[i];

                if (a.Name != b.Name ||
                    a.AttachmentPoint != b.AttachmentPoint ||
                    a.Size.width != b.Size.width ||
                    a.Size.height != b.Size.height)
                {
                    return false;
                }
            }

            return true;
        }
    };
}

namespace RNGOEngine::Resources
{
    // TODO: Extremely ugly function.
    inline std::pair<int, int> GetDesiredAttachmentSize(const AttachmentSize& attachmentSize, const int viewportWidth,
                                                        const int viewportHeight)
    {
        switch (attachmentSize.SizeType)
        {
            case AttachmentSizeType::Absolute:
                return std::make_pair(attachmentSize.width, attachmentSize.height);
            case AttachmentSizeType::PercentOfScreen:
                return std::make_pair(
                    static_cast<int>(
                        static_cast<float>(viewportWidth) *
                        (static_cast<float>(attachmentSize.width) / 100.0f)),
                    static_cast<int>(
                        static_cast<float>(viewportHeight) * (
                            static_cast<float>(attachmentSize.height) / 100.0f))
                );
            default:
                RNGO_ASSERT(false && "GetDesiredAttachmentSize - Unsupported AttachmentSizeType");
        }
    }
}

// Hash Function for RenderTargetSpecification
namespace std
{
    template<>
    struct hash<RNGOEngine::Resources::RenderTargetSpecification>
    {
        size_t operator()(const RNGOEngine::Resources::RenderTargetSpecification& spec) const noexcept
        {
            // TODO: I don't like this.
            auto nameHash = std::hash<std::string>{}(spec.Name);
            size_t attachmentsHash = 0;
            for (const auto& attachment : spec.Attachments)
            {
                attachmentsHash = RNGOEngine::Utilities::Hash::CombineHashes(
                    attachmentsHash, std::hash<std::string>{}(attachment.Name));
                attachmentsHash = RNGOEngine::Utilities::Hash::CombineHashes(
                    attachmentsHash, std::hash<int>{}(static_cast<int>(attachment.AttachmentPoint)));
                attachmentsHash = RNGOEngine::Utilities::Hash::CombineHashes(
                    attachmentsHash, std::hash<std::size_t>{}(attachment.Size.width));
                attachmentsHash = RNGOEngine::Utilities::Hash::CombineHashes(
                    attachmentsHash, std::hash<std::size_t>{}(attachment.Size.height));
            }

            return RNGOEngine::Utilities::Hash::CombineHashes(nameHash, attachmentsHash);
        }
    };
}