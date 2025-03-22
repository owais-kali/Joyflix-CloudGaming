#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <vulkan/vulkan.h>

// Function pointers for real Vulkan functions
static PFN_vkQueuePresentKHR real_vkQueuePresentKHR = nullptr;
static PFN_vkGetInstanceProcAddr real_vkGetInstanceProcAddr = nullptr;
static PFN_vkGetDeviceProcAddr real_vkGetDeviceProcAddr = nullptr;

// Hooked function for vkQueuePresentKHR
VkResult vkQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) {
    printf("[Hook] Image Rendered! Forwarding to real Vulkan.\n");

    // Ensure the real function is valid before calling
    if (!real_vkQueuePresentKHR) {
        fprintf(stderr, "[Hook] Error: real_vkQueuePresentKHR is NULL!\n");
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    return real_vkQueuePresentKHR(queue, pPresentInfo);
}

// Hooked vkGetInstanceProcAddr
extern "C" VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(
    VkInstance instance, const char* pName) {

    if (!real_vkGetInstanceProcAddr) {
        void* handle = dlopen("libvulkan.so.1", RTLD_LAZY);
        if (!handle) {
            fprintf(stderr, "Failed to load Vulkan library\n");
            return nullptr;
        }
        real_vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)dlsym(handle, "vkGetInstanceProcAddr");
        if (!real_vkGetInstanceProcAddr) {
            fprintf(stderr, "Failed to find vkGetInstanceProcAddr\n");
            return nullptr;
        }
    }

    PFN_vkVoidFunction func = real_vkGetInstanceProcAddr(instance, pName);

    if (strcmp(pName, "vkQueuePresentKHR") == 0) {
        real_vkQueuePresentKHR = (PFN_vkQueuePresentKHR)func;  // Store the real function pointer
        return (PFN_vkVoidFunction)vkQueuePresentKHR;
    }

    return func;
}

// Hooked vkGetDeviceProcAddr
extern "C" VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(
    VkDevice device, const char* pName) {

    if (!real_vkGetDeviceProcAddr) {
        void* handle = dlopen("libvulkan.so.1", RTLD_LAZY);
        if (!handle) {
            fprintf(stderr, "Failed to load Vulkan library\n");
            return nullptr;
        }
        real_vkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)dlsym(handle, "vkGetDeviceProcAddr");
        if (!real_vkGetDeviceProcAddr) {
            fprintf(stderr, "Failed to find vkGetDeviceProcAddr\n");
            return nullptr;
        }
    }

    PFN_vkVoidFunction func = real_vkGetDeviceProcAddr(device, pName);

    if (strcmp(pName, "vkQueuePresentKHR") == 0) {
        real_vkQueuePresentKHR = (PFN_vkQueuePresentKHR)func;  // Store the real function pointer
        return (PFN_vkVoidFunction)vkQueuePresentKHR;
    }

    return func;
}
