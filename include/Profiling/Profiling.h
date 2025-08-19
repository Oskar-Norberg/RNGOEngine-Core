//
// Created by Oskar.Norberg on 2025-08-12.
//

#pragma once

#ifdef TRACY_ENABLE

#include <tracy/Tracy.hpp>

#define RNGO_FRAME_MARK FrameMark
#define RNGO_ZONE_SCOPE ZoneScoped
// String View
#define RNGO_ZONE_NAME_VIEW(name) ZoneName(name.data(), name.size())
// String Literal
#define RNGO_ZONE_NAME_C(name) ZoneName(name, sizeof(name) - 1)

#else

#define RNGO_FRAME_MARK 
#define RNGO_ZONE_SCOPE 
// String View
#define RNGO_ZONE_NAME_VIEW(name) 
// String Literal
#define RNGO_ZONE_NAME_C(name) 

#endif