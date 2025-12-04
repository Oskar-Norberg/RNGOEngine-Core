//
// Created by Oskar.Norberg on 2025-12-04.
//

#pragma once

#ifdef WIN32
#include <windows.h>
#endif

namespace RNGOEngine::Utilities::Memory
{
    struct MemoryUsage
    {
        size_t currentUsage = 0;
        size_t available = 0;
        size_t total = 0;
    };

#ifdef WIN32
    static MemoryUsage GetStackMemoryWin32()
    {
        ULONG_PTR stackLow, stackHigh;
        GetCurrentThreadStackLimits(&stackLow, &stackHigh);

#if defined(_MSC_VER)
        ULONG_PTR current = reinterpret_cast<ULONG_PTR>(_AddressOfReturnAddress());
#else
        ULONG_PTR current = reinterpret_cast<ULONG_PTR>(__builtin_frame_address(0));
#endif

        const size_t usedBytes = stackHigh - current;
        const size_t totalBytes = stackHigh - stackLow;

        const MemoryUsage usage{
            .currentUsage = usedBytes, .available = totalBytes - usedBytes, .total = totalBytes
        };

        return usage;
    };
#endif
}