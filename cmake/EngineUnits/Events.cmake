set(EVENTS_PROJECT_NAME RNGOEngine_Events)
add_library(${EVENTS_PROJECT_NAME} INTERFACE
        include/EventQueue/EventQueue.h
        include/EventQueue/EngineEvents/EngineEvents.h
)
target_include_directories(${EVENTS_PROJECT_NAME} INTERFACE include)