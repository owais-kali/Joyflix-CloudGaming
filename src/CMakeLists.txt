#LibCPP OBJS
file(GLOB_RECURSE LibCPP_OBJS
        ${WEBRTC_BUILD}/obj/buildtools/third_party/libc++/libc++/*.o
        )
#LibCPP_ABI OBJS
file(GLOB_RECURSE LibCPP_ABI_OBJS
        ${WEBRTC_BUILD}/obj/buildtools/third_party/libc++abi/libc++abi/*.o
        )

file(GLOB_RECURSE WEBRTC_HEADER
        ${WEBRTC_SRC}/*.h
        )

add_executable(VCNW)
add_subdirectory(Vulkan)
add_subdirectory(Cuda)
add_subdirectory(Webrtc)

target_sources(VCNW
        PRIVATE
        pch.cpp
        pch.h
        WebRTC_Handler.cpp
        WebRTC_Handler.h
        main.cpp)

target_link_libraries(VCNW
        VulkanBase
        CUDA_LIB
        WebRTC
        ${LibCPP_OBJS}
        ${LibCPP_ABI_OBJS})

target_link_options(VCNW
        PUBLIC
        -lX11
)
