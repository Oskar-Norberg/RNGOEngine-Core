//
// Created by Oskar.Norberg on 2025-11-25.
//

#pragma once

#include "Logger.h"
#include "spdlog/sinks/base_sink.h"

namespace RNGOEngine::Core
{
    // This will store every single message in a vector.
    // This is mainly used for drawing to ImGui.
    // This will destroy memory if there are too many messages.
    class VectorSink : public spdlog::sinks::base_sink<std::mutex>
    {
    public:
        std::span<const LogEntry> GetLogs() const
        {
            return m_logs;
        }

    protected:
        void sink_it_(const spdlog::details::log_msg& msg) override
        {
            const char* file = msg.source.filename ? msg.source.filename : "Unknown";
            const char* function = msg.source.funcname ? msg.source.funcname : "Unknown";
            const size_t Line = msg.source.line;

            const LogLocation location{
                .File = file,
                .Function = function,
                .Line = Line
            };
            
            LogEntry entry{
                .Level = SPDLogLevelToRNGOLevel(msg.level),
                .Message = std::string(msg.payload.data(), msg.payload.size()),
                .Location = location,
                .TimePoint = msg.time
            };
            
            m_logs.push_back(std::move(entry));
        }

        void flush_() override
        {
        }

    private:
        std::vector<LogEntry> m_logs;
    };
}
