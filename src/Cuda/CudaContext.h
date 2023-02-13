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

    CUresult Init();
    void Shutdown();

private:
    CUcontext m_context;
    CUuuid id = {};
    bool GetPhysicalDeviceUUIDInto(
            VkInstance instance, VkPhysicalDevice phyDevice, std::array<uint8_t, VK_UUID_SIZE>* deviceUUID);

public:
    CUuuid GetUUID(){
        return id;
    }
};