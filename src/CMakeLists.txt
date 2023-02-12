add_executable(VCNW)
add_subdirectory(Vulkan)
add_subdirectory(Cuda)

target_sources(VCNW
        PRIVATE
        main.cpp)

target_link_libraries(VCNW VulkanBase)
