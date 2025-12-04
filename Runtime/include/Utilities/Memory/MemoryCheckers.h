//
// Created by Oskar.Norberg on 2025-12-04.
//

#pragma once

#include "MemoryChecker.impl.h"

namespace RNGOEngine::Utilities::Memory
{
    static MemoryUsage GetStackMemoryUsage()
    {
#ifdef WIN32
        return GetStackMemoryWin32();
#else
        // TODO: Implement for other platforms
        return MemoryUsage{};
#endif
    }

    static MemoryUsage GetHeapMemoryUsage()
    {
#ifdef WIN32
        return GetHeapMemoryWin32();
#else
        // TODO: Implement for other platforms
        return MemoryUsage{};
#endif
    }
}