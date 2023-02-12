#pragma once
#include <cuda.h>
#include <vulkan/vulkan.h>

#include "../pch.h"

// The minimum version of CUDA Toolkit
const int kRequiredDriverVersion = 11000;

class CudaContext {
public:
    CudaContext();
    ~CudaContext() = default;

    CUresult Init(const VkInstance instance, VkPhysicalDevice physicalDevice);
    void Shutdown();

private:
    CUcontext m_context;

    bool GetPhysicalDeviceUUIDInto(
            VkInstance instance, VkPhysicalDevice phyDevice, std::array<uint8_t, VK_UUID_SIZE>* deviceUUID);
};