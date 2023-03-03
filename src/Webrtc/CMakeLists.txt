set(CMAKE_CXX_COMPILER ${WEBRTC_SRC}/third_party/llvm-build/Release+Asserts/bin/clang)

#JSON_CPP
file(GLOB_RECURSE JSON_CPP_OBJS
        ${WEBRTC_BUILD}/obj/third_party/jsoncpp/jsoncpp/
        ${WEBRTC_BUILD}/obj/third_party/jsoncpp/jsoncpp/*.o
        )

add_library(WebRTC STATIC)

target_compile_definitions(WebRTC PRIVATE WEBRTC_LINUX WEBRTC_POSIX)

target_sources(WebRTC
        PRIVATE
        pch.cpp
        pch.h
        PeerConnectionObject.cpp
        PeerConnectionObject.h
        VideoTrackSource.cpp
        VideoTrackSource.h
        VideoFrame.cpp
        VideoFrameX.h
        Size.h
        Types.h
        Logger.cpp
        Logger.h
        Context.cpp
        Context.h
        CreateSessionDescriptionObserverX.h
        CreateSessionDescriptionObserverX.cpp
        DataChannelObject.cpp
        DataChannelObject.h
        SetLocalDescriptionObserver.cpp
        SetLocalDescriptionObserver.h
        SetRemoteDescriptionObserver.cpp
        SetRemoteDescriptionObserver.h
        MediaStreamObserverX.cpp
        MediaStreamObserverX.h
        PeerConnectionStatsCollectorCallback.cpp
        PeerConnectionStatsCollectorCallback.h
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
        ${WEBRTC_BUILD}/obj/libwebrtc.a
        ${JSON_CPP_OBJS}
        )