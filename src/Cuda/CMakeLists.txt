add_executable(CUDA_App)
find_package(Vulkan REQUIRED)

target_sources(CUDA_App
        PRIVATE
        CudaApp.cpp
        CudaContext.cpp
        CudaContext.h
        ../pch.cpp
        ../pch.h)

message("vulkan lib:" ${Vulkan_LIBRARY})
target_link_libraries(CUDA_App
        PRIVATE
        ${CUDA_CUDA_LIBRARY}
        ${Vulkan_LIBRARY})