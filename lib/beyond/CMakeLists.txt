cmake_minimum_required(VERSION 3.15)
project(beyond VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)  # or 14, 17, 20 depending on your needs
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC
)


add_subdirectory(src/argument)
add_subdirectory(src/job)
add_subdirectory(src/task)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../util util)
add_subdirectory(sdk)

set(FFMPEG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sdk/FFmpeg)
set(FFMPEG_LIB_DIR ${FFMPEG_DIR}/lib)
set(FFMPEG_INCLUDE_DIR ${FFMPEG_DIR}/include)

# include 
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include         # headers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/job/include
    
    ${CMAKE_CURRENT_SOURCE_DIR}/../../util/include
    ${FFMPEG_DIR}/include

)

target_link_libraries(${PROJECT_NAME} PUBLIC ${UTIL_LIBRARY} kamsi 
${FFMPEG_LIBRARIES} 
pthread
)



if(CMAKE_BUILD_TYPE STREQUAL "Debug")
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "${FFMPEG_LIB_DIR}"
    INSTALL_RPATH_USE_LINK_PATH TRUE
)
endif()