//
// Created by Oskar.Norberg on 2025-11-25.
//

#pragma once

#include "spdlog/sinks/base_sink.h"

namespace RNGOEngine::Core
{
    // This will store every single message in a vector.
    // This is mainly used for drawing to ImGui.
    // This will destroy memory if there are too many messages.
    class VectorSink : public spdlog::sinks::base_sink<std::mutex>
    {
    public:
        std::span<const std::string> GetLogs() const;

    protected:
        void sink_it_(const spdlog::details::log_msg& msg) override
        {
            m_logs.emplace_back(msg.payload.data(), msg.payload.size());
        }
        
        void flush_() override
        {
        }

    private:
        std::vector<std::string> m_logs;
    };
}
