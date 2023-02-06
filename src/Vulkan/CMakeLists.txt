add_library(VulkanBase STATIC)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_definitions(VulkanBase PRIVATE _VALIDATION)
endif()

target_sources(VulkanBase
        PRIVATE
        VulkanBase.cpp
        VulkanBase.h
        VulkanTools.cpp
        VulkanTools.h
        VulkanInitializers.hpp
        CommandLineParser.hpp
        VulkanDevice.cpp
        VulkanDevice.h
        VulkanBuffer.cpp
        VulkanBuffer.h
        VulkanSwapChain.cpp
        VulkanSwapChain.h)
find_package(Vulkan REQUIRED)

target_link_libraries(VulkanBase
        PRIVATE
        ${Vulkan_LIBRARY}
        )