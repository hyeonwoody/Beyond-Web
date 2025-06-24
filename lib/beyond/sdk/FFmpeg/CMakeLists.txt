cmake_minimum_required(VERSION 3.15)

project(ffmpeg_sdk LANGUAGES C)
set(FFMPEG_DIR ${CMAKE_CURRENT_SOURCE_DIR})
# Add include path for headers
set(FFMPEG_INCLUDE_DIR ${FFMPEG_DIR}/include)
set(FFMPEG_LIB_DIR ${FFMPEG_DIR}/lib)

include_directories(
    ${FFMPEG_INCLUDE_DIR}
)

link_directories(${FFMPEG_LIB_DIR})

# Create imported targets for FFmpeg shared libraries
macro(import_ffmpeg_lib name versioned_so)
    add_library(ffmpeg::${name} SHARED IMPORTED GLOBAL)
    set_target_properties(ffmpeg::${name} PROPERTIES
        IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/${versioned_so}
        INTERFACE_INCLUDE_DIRECTORIES ${FFMPEG_INCLUDE_DIR}/lib${name}
    )
endmacro()

import_ffmpeg_lib(avformat libavformat.so.62.1.100)
import_ffmpeg_lib(avutil libavutil.so.60.3.100)
import_ffmpeg_lib(avcodec libavcodec.so.62.4.100)
import_ffmpeg_lib(swresample libswresample.so.6.0.100)
import_ffmpeg_lib(swscale libswscale.so.9.0.100)

set(FFMPEG_LIBRARIES
    ffmpeg::avformat
    ffmpeg::avutil
    ffmpeg::avcodec
    ffmpeg::swresample
    ffmpeg::swscale
    CACHE INTERNAL "List of FFmpeg library targets"
)