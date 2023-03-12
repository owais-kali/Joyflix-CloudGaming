cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
project(
        VCNW
        VERSION 2.4.0
        LANGUAGES C CXX ASM)

cmake_policy(SET CMP0076 NEW)

option(WEBRTC_DISBALE "Disable WebRTC to speed up project loading for development" ON)
option(WEBRTC_SRC "Set WEBRTC_SRC path by -DWEBRTC_SRC eg: -DWEBRTC_SRC=Webrtc_Source_Code/webrtc-checkout/src")

if(NOT WEBRTC_SRC)
    message( SEND_ERROR "Set WEBRTC_SRC path by -DWEBRTC_SRC eg: -DWEBRTC_SRC=Webrtc_Source_Code/webrtc-checkout/src" )
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(WEBRTC_BUILD ${WEBRTC_SRC}/out/Debug)
    add_compile_definitions(_DEBUG)
else()
    set(WEBRTC_BUILD ${WEBRTC_SRC}/out/Release)
endif()

find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(WEBSOCKETPP REQUIRED)
find_package(CUDA REQUIRED)
find_package(Vulkan REQUIRED)

add_subdirectory(NvCodec)
add_subdirectory(src)
