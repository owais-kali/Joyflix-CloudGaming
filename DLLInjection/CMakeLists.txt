add_library(vulkan_hook SHARED vulkan_hook.cpp)

target_link_libraries(vulkan_hook PRIVATE Vulkan::Vulkan dl)