cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
project(
        VCNW
        VERSION 2.4.0
        LANGUAGES C CXX ASM)

cmake_policy(SET CMP0076 NEW)

if(NOT DEFINED WEBRTC_SRC)
    message( SEND_ERROR "Set WEBRTC_SRC path by -DWEBRTC_SRC eg: -DWEBRTC_SRC=Webrtc_Source_Code/webrtc-checkout/src" )
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(WEBRTC_BUILD ${WEBRTC_SRC}/out/Debug)
else()
    set(WEBRTC_BUILD ${WEBRTC_SRC}/out/Release)
endif()

find_package(CUDA REQUIRED)
find_package(Vulkan REQUIRED)

add_subdirectory(NvCodec)
add_subdirectory(src)