# util 라이브러리 정의
file(GLOB UTIL_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(util_lib STATIC ${UTIL_SOURCES})

# include 경로 설정
target_include_directories(util_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

