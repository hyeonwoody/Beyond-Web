cmake_minimum_required(VERSION 3.15)
project(beyond-web VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)  # or 14, 17, 20 depending on your needs
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type default to Debug (can be overridden with cmake command line)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
endif()


file(GLOB KAMSI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/kamsi/src/*.cpp")
file(GLOB UTIL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/util/src/*.cpp")

add_subdirectory(kamsi)

add_library(util STATIC ${UTIL_SOURCES})
target_include_directories(util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/util/include)

# ✅ beyond 라이브러리 서브디렉토리로 추가
add_subdirectory(lib/beyond)

# ✅ 실행파일 정의
add_executable(${PROJECT_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/beyond/src/Beyond.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/beyond/include
)
# ✅ 라이브러리 연결
target_link_libraries(${PROJECT_NAME} PRIVATE beyond kamsi util)
