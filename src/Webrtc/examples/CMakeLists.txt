add_executable(peerconnection_client)
target_sources(peerconnection_client
        PRIVATE
        examples/peerconnection/client/conductor.cc
        examples/peerconnection/client/defaults.cc
        examples/peerconnection/client/linux/main.cc
        examples/peerconnection/client/linux/main_wnd.cc
        examples/peerconnection/client/peer_connection_client.cc)

target_compile_definitions(peerconnection_client PRIVATE WEBRTC_LINUX WEBRTC_POSIX)

target_include_directories(peerconnection_client SYSTEM PRIVATE ${WEBRTC_SRC})
target_include_directories(peerconnection_client SYSTEM PRIVATE ${WEBRTC_SRC}/buildtools/third_party/libc++/trunk/include)
target_include_directories(peerconnection_client SYSTEM PRIVATE ${WEBRTC_SRC}/third_party/perfetto/buildtools/libcxx_config)
target_include_directories(peerconnection_client SYSTEM PRIVATE ${WEBRTC_SRC}/third_party/abseil-cpp/)
target_include_directories(peerconnection_client SYSTEM PRIVATE ${WEBRTC_SRC}/tools/clang/plugins/tests/system/)
target_include_directories(peerconnection_client SYSTEM PRIVATE ${WEBRTC_SRC}third_party/crashpad/crashpad/compat/non_win/)
target_include_directories(peerconnection_client SYSTEM PRIVATE ${WEBRTC_SRC}/build/linux/debian_bullseye_amd64-sysroot/usr/include/glib-2.0/)
target_include_directories(peerconnection_client SYSTEM PRIVATE ${WEBRTC_SRC}/build/linux/debian_bullseye_amd64-sysroot/usr/lib/x86_64-linux-gnu/glib-2.0/include/)
target_include_directories(peerconnection_client SYSTEM PRIVATE ${WEBRTC_SRC}/build/linux/debian_bullseye_amd64-sysroot/usr/include/gtk-4.0/)
target_include_directories(peerconnection_client SYSTEM PRIVATE ${WEBRTC_SRC}/build/linux/debian_bullseye_amd64-sysroot/usr/include/cairo/)
target_include_directories(peerconnection_client SYSTEM PRIVATE ${WEBRTC_SRC}/build/linux/debian_bullseye_amd64-sysroot/usr/include/pango-1.0/)
target_include_directories(peerconnection_client SYSTEM PRIVATE ${WEBRTC_SRC}/build/linux/debian_bullseye_amd64-sysroot/usr/include/harfbuzz/)
target_include_directories(peerconnection_client SYSTEM PRIVATE ${WEBRTC_SRC}/build/linux/debian_bullseye_amd64-sysroot/usr/include/gdk-pixbuf-2.0/)
target_include_directories(peerconnection_client SYSTEM PRIVATE ${WEBRTC_SRC}/build/linux/debian_bullseye_amd64-sysroot/usr/include/graphene-1.0/)
target_include_directories(peerconnection_client SYSTEM PRIVATE ${WEBRTC_SRC}/build/linux/debian_bullseye_amd64-sysroot/usr/lib/x86_64-linux-gnu/graphene-1.0/include/)