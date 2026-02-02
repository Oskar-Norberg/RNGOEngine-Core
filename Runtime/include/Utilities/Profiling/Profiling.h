//
// Created by Oskar.Norberg on 2025-08-12.
//

#pragma once

#ifdef RNGOENGINE_TRACY_ENABLE

#include <tracy/Tracy.hpp>

// Has to be a string literal
#define RNGO_ZONE_SCOPED_N(name) ZoneScopedN(name)

#define RNGO_FRAME_MARK FrameMark

#else

#define RNGO_ZONE_SCOPED_N(name)

#define RNGO_FRAME_MARK

#endif