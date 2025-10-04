set(EVENTS_PROJECT_NAME RNGOEngine_Events)
add_library(${EVENTS_PROJECT_NAME} INTERFACE
        include/EventQueue/EventQueue.h
        include/EventQueue/EngineEvents/EngineEvents.h
)
target_include_directories(${EVENTS_PROJECT_NAME} INTERFACE include)
target_link_libraries(${EVENTS_PROJECT_NAME} INTERFACE ${PROFILING_PROJECT_NAME} ${DATA_PROJECT_NAME})
