include(cmake/EngineUnits/Data.cmake)

include(cmake/EngineUnits/Profiling.cmake)

include(cmake/EngineUnits/Concepts.cmake)
include(cmake/EngineUnits/Math.cmake)
include(cmake/EngineUnits/Utilities.cmake)
include(cmake/EngineUnits/Logging.cmake)
include(cmake/EngineUnits/Error.cmake)

include(cmake/EngineUnits/Containers.cmake)
include(cmake/EngineUnits/Events.cmake)
include(cmake/EngineUnits/InputManager.cmake)

include(cmake/EngineUnits/Threading.cmake)

include(cmake/EngineUnits/Windowing.cmake)
include(cmake/EngineUnits/Rendering.cmake)

include(cmake/EngineUnits/ResourceManagement.cmake)
include(cmake/EngineUnits/AssetManagement.cmake)

include(cmake/EngineUnits/Components.cmake)
include(cmake/EngineUnits/Scene.cmake)
include(cmake/EngineUnits/Systems.cmake)

include(cmake/EngineUnits/Physics.cmake)
include(cmake/EngineUnits/RenderAPI.cmake)

list(APPEND ALL_ENGINE_UNITS
        ${DATA_PROJECT_NAME}

        ${PROFILING_PROJECT_NAME}

        ${CONCEPTS_PROJECT_NAME}
        ${MATH_PROJECT_NAME}
        ${UTILITIES_PROJECT_NAME}
        ${LOGGING_PROJECT_NAME}
        ${ERROR_PROJECT_NAME}

        ${CONTAINERS_PROJECT_NAME}
        ${EVENTS_PROJECT_NAME}
        ${INPUT_MANAGER_PROJECT_NAME}

        ${JOBS_PROJECT_NAME}

        ${WINDOWING_PROJECT_NAME}
        ${RENDERING_PROJECT_NAME}

        ${RESOURCE_MANAGEMENT_PROJECT_NAME}
        ${ASSET_MANAGER_PROJECT_NAME}

        ${SCENE_PROJECT_NAME}
        ${SYSTEM_PROJECT_NAME}
        ${COMPONENT_PROJECT_NAME}

        ${PHYSICS_SYSTEM_PROJECT_NAME}
        ${RENDER_API_PROJECT_NAME}
)

set(ALL_ENGINE_UNITS "${ALL_ENGINE_UNITS}" PARENT_SCOPE)