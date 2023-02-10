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
find_package(Threads REQUIRED)
find_package(XCB REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_XCB_KHR")

target_link_libraries(VulkanBase
        PRIVATE
        ${Vulkan_LIBRARY}
        ${XCB_LIBRARIES}
        )