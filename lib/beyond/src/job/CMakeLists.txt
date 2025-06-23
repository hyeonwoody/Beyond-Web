set(JOB_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(JOB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/JobFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Jobs.cpp
)

if(ENABLE_MOVE)
    message(STATUS "ENABLE_MOVE requires ENABLE_COPY — enabling ENABLE_COPY automatically.")
    set(ENABLE_COPY ON CACHE BOOL "Force-enabled because MOVE requires COPY" FORCE)
    list(APPEND JOB_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/move/src/Move.cpp
    )
    list(APPEND JOB_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/move/include
    )
    target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_MOVE)
endif()

    if(ENABLE_COPY)
        list(APPEND JOB_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/src/copy/src/Copy.cpp
        )
        list(APPEND JOB_INCLUDES
            ${CMAKE_CURRENT_SOURCE_DIR}/src/copy/include
        )
        target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_COPY)
    endif()

if(ENABLE_SYMBOLICLINK)
    list(APPEND JOB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/symbolicLink/src/SymbolicLink.cpp
    )
    list(APPEND JOB_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/symbolicLink/include
    )
    target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_SYMBOLICLINK)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${JOB_INCLUDES})
target_sources(${PROJECT_NAME} PRIVATE ${JOB_SOURCES})
