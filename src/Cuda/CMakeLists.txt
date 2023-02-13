add_library(CUDA_LIB SHARED)
find_package(Vulkan REQUIRED)

target_sources(CUDA_LIB
        PRIVATE
        CudaContext.cpp
        CudaContext.h
        ../pch.cpp
        ../pch.h)

target_link_libraries(CUDA_LIB
        PRIVATE
        ${CUDA_CUDA_LIBRARY}
        ${Vulkan_LIBRARY})

#target_compile_definitions(CUDA_LIB PRIVATE WEBRTC_LINUX WEBRTC_POSIX
#        VK_NO_PROTOTYPES)