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

# include 
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include         # headers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/job/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../util/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC ${UTIL_LIBRARY} kamsi)