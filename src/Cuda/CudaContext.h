#pragma once

#include <cuda.h>
#include <vulkan/vulkan.h>

class CudaContext {
public:
    CudaContext();
    ~CudaContext() = default;

    CUresult Init(const VkInstance instance, VkPhysicalDevice physicalDevice);
    void Shutdown();
};