set(CMAKE_CXX_COMPILER ${WEBRTC_SRC}/third_party/llvm-build/Release+Asserts/bin/clang)

#LibCPP OBJS
file(GLOB_RECURSE LibCPP_OBJS
        ${WEBRTC_BUILD}/obj/buildtools/third_party/libc++/libc++/*.o
        )

file(GLOB_RECURSE LibCPP_ABI_OBJS
        ${WEBRTC_BUILD}/obj/buildtools/third_party/libc++abi/libc++abi/*.o
        )

add_library(WebRTC STATIC)

target_compile_definitions(WebRTC PRIVATE WEBRTC_LINUX WEBRTC_POSIX)

target_sources(WebRTC
        PRIVATE
        pch.cpp
        pch.h
        PeerConnectionObject.cpp
        PeerConnectionObject.h
        Logger.cpp
        Logger.h
        Context.cpp
        Context.h
        SSDO.cpp
        SSDO.h
        WebRTCPlugin.cpp
        WebRTCPlugin.h
        API.cpp
        API.h
        )

target_include_directories(WebRTC SYSTEM PRIVATE .)

target_include_directories(WebRTC SYSTEM PRIVATE ${WEBRTC_SRC})
target_include_directories(WebRTC SYSTEM PRIVATE ${WEBRTC_SRC}/buildtools/third_party/libc++/trunk/include)
target_include_directories(WebRTC SYSTEM PRIVATE ${WEBRTC_SRC}/third_party/perfetto/buildtools/libcxx_config)
target_include_directories(WebRTC SYSTEM PRIVATE ${WEBRTC_SRC}/third_party/abseil-cpp/)
target_include_directories(WebRTC SYSTEM PRIVATE ${WEBRTC_SRC}/third_party/jsoncpp/source/include/)

target_link_libraries(WebRTC
        PRIVATE
        ${WEBRTC_SRC}/out/Debug/obj/libwebrtc.a
)

