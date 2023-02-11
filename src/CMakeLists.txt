add_executable(VCNW)
add_subdirectory(Vulkan)

target_sources(VCNW
        PRIVATE
        main.cpp)

target_link_libraries(VCNW VulkanBase)
