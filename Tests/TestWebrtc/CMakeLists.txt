add_executable(TestWebrtc
main.cpp)

target_compile_definitions(TestWebrtc PRIVATE WEBRTC_LINUX WEBRTC_POSIX)

target_link_libraries(TestWebrtc PRIVATE
        ${WEBRTC_BUILD}/obj/libwebrtc.a
)

target_include_directories(TestWebrtc PRIVATE
        /media/owais/LinuxHDD/CloudGaming/com.unity.webrtc/Plugin~/webrtc/include/
        /media/owais/LinuxHDD/CloudGaming/com.unity.webrtc/Plugin~/webrtc/include/third_party/abseil-cpp)