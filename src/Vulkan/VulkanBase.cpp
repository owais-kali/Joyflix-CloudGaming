//
// Created by owais on 6/2/23.
//

#include "VulkanBase.h"

VkResult VulkanBase::createInstance(bool enableValidation)
{
    this->settings.validation = enableValidation;

    // Validation can also be forced via a define
#if defined(_VALIDATION)
    this->settings.validation = true;
#endif

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = name.c_str();
    appInfo.pEngineName = name.c_str();
    appInfo.apiVersion = apiVersion;

    std::vector<const char*> instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };

    // Enable surface extensions depending on os
#if defined(_WIN32)
    instanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
    instanceExtensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#elif defined(_DIRECT2DISPLAY)
    instanceExtensions.push_back(VK_KHR_DISPLAY_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_DIRECTFB_EXT)
    instanceExtensions.push_back(VK_EXT_DIRECTFB_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
    instanceExtensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
    instanceExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_IOS_MVK)
    instanceExtensions.push_back(VK_MVK_IOS_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
    instanceExtensions.push_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_HEADLESS_EXT)
    instanceExtensions.push_back(VK_EXT_HEADLESS_SURFACE_EXTENSION_NAME);
#endif

    // Get extensions supported by the instance and store for later use
    uint32_t extCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
    if (extCount > 0)
    {
        std::vector<VkExtensionProperties> extensions(extCount);
        if (vkEnumerateInstanceExtensionProperties(nullptr, &extCount, &extensions.front()) == VK_SUCCESS)
        {
            for (VkExtensionProperties extension : extensions)
            {
                supportedInstanceExtensions.push_back(extension.extensionName);
            }
        }
    }

#if (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK))
    // SRS - When running on iOS/macOS with MoltenVK, enable VK_KHR_get_physical_device_properties2 if not already enabled by the example (required by VK_KHR_portability_subset)
	if (std::find(enabledInstanceExtensions.begin(), enabledInstanceExtensions.end(), VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME) == enabledInstanceExtensions.end())
	{
		enabledInstanceExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
	}
#endif

    // Enabled requested instance extensions
    if (enabledInstanceExtensions.size() > 0)
    {
        for (const char * enabledExtension : enabledInstanceExtensions)
        {
            // Output message if requested extension is not available
            if (std::find(supportedInstanceExtensions.begin(), supportedInstanceExtensions.end(), enabledExtension) == supportedInstanceExtensions.end())
            {
                std::cerr << "Enabled instance extension \"" << enabledExtension << "\" is not present at instance level\n";
            }
            instanceExtensions.push_back(enabledExtension);
        }
    }

    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = NULL;
    instanceCreateInfo.pApplicationInfo = &appInfo;

#if (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK)) && defined(VK_KHR_portability_enumeration)
    // SRS - When running on iOS/macOS with MoltenVK and VK_KHR_portability_enumeration is defined and supported by the instance, enable the extension and the flag
	if (std::find(supportedInstanceExtensions.begin(), supportedInstanceExtensions.end(), VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME) != supportedInstanceExtensions.end())
	{
		instanceExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
		instanceCreateInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
	}
#endif

    if (instanceExtensions.size() > 0)
    {
        if (settings.validation)
        {
            instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);	// SRS - Dependency when VK_EXT_DEBUG_MARKER is enabled
            instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        instanceCreateInfo.enabledExtensionCount = (uint32_t)instanceExtensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
    }

    // The VK_LAYER_KHRONOS_validation contains all current validation functionality.
    // Note that on Android this layer requires at least NDK r20
    const char* validationLayerName = "VK_LAYER_KHRONOS_validation";
    if (settings.validation)
    {
        // Check if this layer is available at instance level
        uint32_t instanceLayerCount;
        vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
        std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
        vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());
        bool validationLayerPresent = false;
        for (VkLayerProperties layer : instanceLayerProperties) {
            if (strcmp(layer.layerName, validationLayerName) == 0) {
                validationLayerPresent = true;
                break;
            }
        }
        if (validationLayerPresent) {
            instanceCreateInfo.ppEnabledLayerNames = &validationLayerName;
            instanceCreateInfo.enabledLayerCount = 1;
        } else {
            std::cerr << "Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled\n";
        }
    }
    return vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
}

VulkanBase::VulkanBase(bool enableValidation) {

}

bool VulkanBase::initVulkan()
{
    VkResult err;

    // Vulkan instance
    err = createInstance(settings.validation);
    if (err) {
        vks::tools::exitFatal("Could not create Vulkan instance : \n" + vks::tools::errorString(err), err);
        return false;
    }

#if defined(VK_USE_PLATFORM_ANDROID_KHR)
    vks::android::loadVulkanFunctions(instance);
#endif

    // If requested, we enable the default validation layers for debugging
    if (settings.validation)
    {
//        vks::debug::setupDebugging(instance);
    }

    // Physical device
    uint32_t gpuCount = 0;
    // Get number of available physical devices
    VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr));
    if (gpuCount == 0) {
        vks::tools::exitFatal("No device with Vulkan support found", -1);
        return false;
    }
    // Enumerate devices
    std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
    err = vkEnumeratePhysicalDevices(instance, &gpuCount, physicalDevices.data());
    if (err) {
        vks::tools::exitFatal("Could not enumerate physical devices : \n" + vks::tools::errorString(err), err);
        return false;
    }

    // GPU selection

    for (uint32_t i = 0; i < gpuCount; i++) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevices[i], &deviceProperties);
        std::cout << "Device [" << i << "] : " << deviceProperties.deviceName << std::endl;
        std::cout << " Type: " << vks::tools::physicalDeviceTypeString(deviceProperties.deviceType) << "\n";
        std::cout << " API: " << (deviceProperties.apiVersion >> 22) << "." << ((deviceProperties.apiVersion >> 12) & 0x3ff) << "." << (deviceProperties.apiVersion & 0xfff) << "\n";
    }

    // Select physical device to be used for the Vulkan example
    // Defaults to the first device unless specified by command line
    uint32_t selectedDevice = 0;

#if !defined(VK_USE_PLATFORM_ANDROID_KHR)
    // GPU selection via command line argument
    if (commandLineParser.isSet("gpuselection")) {
        uint32_t index = commandLineParser.getValueAsInt("gpuselection", 0);
        if (index > gpuCount - 1) {
            std::cerr << "Selected device index " << index << " is out of range, reverting to device 0 (use -listgpus to show available Vulkan devices)" << "\n";
        } else {
            selectedDevice = index;
        }
    }
    if (commandLineParser.isSet("gpulist")) {
        std::cout << "Available Vulkan devices" << "\n";
        for (uint32_t i = 0; i < gpuCount; i++) {
            VkPhysicalDeviceProperties deviceProperties;
            vkGetPhysicalDeviceProperties(physicalDevices[i], &deviceProperties);
            std::cout << "Device [" << i << "] : " << deviceProperties.deviceName << std::endl;
            std::cout << " Type: " << vks::tools::physicalDeviceTypeString(deviceProperties.deviceType) << "\n";
            std::cout << " API: " << (deviceProperties.apiVersion >> 22) << "." << ((deviceProperties.apiVersion >> 12) & 0x3ff) << "." << (deviceProperties.apiVersion & 0xfff) << "\n";
        }
    }
#endif

    return true;
}

