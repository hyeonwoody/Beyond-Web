file(GLOB_RECURSE TASK_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

target_sources(${PROJECT_NAME} PRIVATE ${TASK_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/put
    ${CMAKE_CURRENT_SOURCE_DIR}/include/code
    ${CMAKE_CURRENT_SOURCE_DIR}/include/multiMux
)