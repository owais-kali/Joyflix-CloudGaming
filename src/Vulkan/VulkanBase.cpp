#include "VulkanBase.h"

std::vector<const char*> VulkanBase::args;

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
    settings.validation = enableValidation;

    // Command line arguments
    commandLineParser.add("help", { "--help" }, 0, "Show help");
    commandLineParser.add("validation", { "-v", "--validation" }, 0, "Enable validation layers");
    commandLineParser.add("vsync", { "-vs", "--vsync" }, 0, "Enable V-Sync");
    commandLineParser.add("fullscreen", { "-f", "--fullscreen" }, 0, "Start in fullscreen mode");
    commandLineParser.add("width", { "-w", "--width" }, 1, "Set window width");
    commandLineParser.add("height", { "-h", "--height" }, 1, "Set window height");
    commandLineParser.add("shaders", { "-s", "--shaders" }, 1, "Select shader type to use (glsl or hlsl)");
    commandLineParser.add("gpuselection", { "-g", "--gpu" }, 2, "Select GPU to run on");
    commandLineParser.add("gpulist", { "-gl", "--listgpus" }, 0, "Display a list of available Vulkan devices");
    commandLineParser.add("benchmark", { "-b", "--benchmark" }, 0, "Run example in benchmark mode");
    commandLineParser.add("benchmarkwarmup", { "-bw", "--benchwarmup" }, 1, "Set warmup time for benchmark mode in seconds");
    commandLineParser.add("benchmarkruntime", { "-br", "--benchruntime" }, 1, "Set duration time for benchmark mode in seconds");
    commandLineParser.add("benchmarkresultfile", { "-bf", "--benchfilename" }, 1, "Set file name for benchmark results");
    commandLineParser.add("benchmarkresultframes", { "-bt", "--benchframetimes" }, 0, "Save frame times to benchmark results file");
    commandLineParser.add("benchmarkframes", { "-bfs", "--benchmarkframes" }, 1, "Only render the given number of frames");

    commandLineParser.parse(args);

    initxcbConnection();
}
VulkanBase::~VulkanBase(){}

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

    physicalDevice = physicalDevices[selectedDevice];

    // Store properties (including limits), features and memory properties of the physical device (so that examples can check against them)
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &deviceMemoryProperties);

    // Derived examples can override this to set actual features (based on above readings) to enable for logical device creation
    getEnabledFeatures();

    // Vulkan device creation
    // This is handled by a separate class that gets a logical device representation
    // and encapsulates functions related to a device
    vulkanDevice = new vks::VulkanDevice(physicalDevice);

    // Derived examples can enable extensions based on the list of supported extensions read from the physical device
    getEnabledExtensions();

    VkResult res = vulkanDevice->createLogicalDevice(enabledFeatures, enabledDeviceExtensions, deviceCreatepNextChain);
    if (res != VK_SUCCESS) {
        vks::tools::exitFatal("Could not create Vulkan device: \n" + vks::tools::errorString(res), res);
        return false;
    }
    device = vulkanDevice->logicalDevice;

    // Get a graphics queue from the device
    vkGetDeviceQueue(device, vulkanDevice->queueFamilyIndices.graphics, 0, &queue);

    // Find a suitable depth format
    VkBool32 validDepthFormat = vks::tools::getSupportedDepthFormat(physicalDevice, &depthFormat);
    assert(validDepthFormat);

    swapChain.connect(instance, physicalDevice, device);

    // Create synchronization objects
    VkSemaphoreCreateInfo semaphoreCreateInfo = vks::initializers::semaphoreCreateInfo();
    // Create a semaphore used to synchronize image presentation
    // Ensures that the image is displayed before we start submitting new commands to the queue
    VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &semaphores.presentComplete));
    // Create a semaphore used to synchronize command submission
    // Ensures that the image is not presented until all commands have been submitted and executed
    VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &semaphores.renderComplete));

    // Set up submit info structure
    // Semaphores will stay the same during application lifetime
    // Command buffer submission info is set by each example
    submitInfo = vks::initializers::submitInfo();
    submitInfo.pWaitDstStageMask = &submitPipelineStages;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &semaphores.presentComplete;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &semaphores.renderComplete;

    return true;
}

static inline xcb_intern_atom_reply_t* intern_atom_helper(xcb_connection_t *conn, bool only_if_exists, const char *str)
{
    xcb_intern_atom_cookie_t cookie = xcb_intern_atom(conn, only_if_exists, strlen(str), str);
    return xcb_intern_atom_reply(conn, cookie, NULL);
}
std::string VulkanBase::getWindowTitle()
{
    std::string device(deviceProperties.deviceName);
    std::string windowTitle;
    windowTitle = title + " - " + device;
    if (!settings.overlay) {
        windowTitle += " - " + std::to_string(frameCounter) + " fps";
    }
    return windowTitle;
}
// Set up a window using XCB and request event types
xcb_window_t VulkanBase::setupWindow() {
    uint32_t value_mask, value_list[32];

    window = xcb_generate_id(connection);

    value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    value_list[0] = screen->black_pixel;
    value_list[1] =
            XCB_EVENT_MASK_KEY_RELEASE |
            XCB_EVENT_MASK_KEY_PRESS |
            XCB_EVENT_MASK_EXPOSURE |
            XCB_EVENT_MASK_STRUCTURE_NOTIFY |
            XCB_EVENT_MASK_POINTER_MOTION |
            XCB_EVENT_MASK_BUTTON_PRESS |
            XCB_EVENT_MASK_BUTTON_RELEASE;

    if (settings.fullscreen)
    {
        width = destWidth = screen->width_in_pixels;
        height = destHeight = screen->height_in_pixels;
    }

    xcb_create_window(connection,
                      XCB_COPY_FROM_PARENT,
                      window, screen->root,
                      0, 0, width, height, 0,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT,
                      screen->root_visual,
                      value_mask, value_list);

    /* Magic code that will send notification when window is destroyed */
    xcb_intern_atom_reply_t* reply = intern_atom_helper(connection, true, "WM_PROTOCOLS");
    atom_wm_delete_window = intern_atom_helper(connection, false, "WM_DELETE_WINDOW");

    xcb_change_property(connection, XCB_PROP_MODE_REPLACE,
                        window, (*reply).atom, 4, 32, 1,
                        &(*atom_wm_delete_window).atom);

    std::string windowTitle = getWindowTitle();
    xcb_change_property(connection, XCB_PROP_MODE_REPLACE,
                        window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
                        title.size(), windowTitle.c_str());

    free(reply);

    /**
     * Set the WM_CLASS property to display
     * title in dash tooltip and application menu
     * on GNOME and other desktop environments
     */
    std::string wm_class;
    wm_class = wm_class.insert(0, name);
    wm_class = wm_class.insert(name.size(), 1, '\0');
    wm_class = wm_class.insert(name.size() + 1, title);
    wm_class = wm_class.insert(wm_class.size(), 1, '\0');
    xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window, XCB_ATOM_WM_CLASS, XCB_ATOM_STRING, 8, wm_class.size() + 2, wm_class.c_str());

    if (settings.fullscreen)
    {
        xcb_intern_atom_reply_t *atom_wm_state = intern_atom_helper(connection, false, "_NET_WM_STATE");
        xcb_intern_atom_reply_t *atom_wm_fullscreen = intern_atom_helper(connection, false, "_NET_WM_STATE_FULLSCREEN");
        xcb_change_property(connection,
                            XCB_PROP_MODE_REPLACE,
                            window, atom_wm_state->atom,
                            XCB_ATOM_ATOM, 32, 1,
                            &(atom_wm_fullscreen->atom));
        free(atom_wm_fullscreen);
        free(atom_wm_state);
    }

    xcb_map_window(connection, window);

    return(window);
}

void VulkanBase::initxcbConnection(){
    const xcb_setup_t *setup;
    xcb_screen_iterator_t iter;
    int scr;

    // xcb_connect always returns a non-NULL pointer to a xcb_connection_t,
    // even on failure. Callers need to use xcb_connection_has_error() to
    // check for failure. When finished, use xcb_disconnect() to close the
    // connection and free the structure.
    connection = xcb_connect(NULL, &scr);
    assert( connection );
    if( xcb_connection_has_error(connection) ) {
        printf("Could not find a compatible Vulkan ICD!\n");
        fflush(stdout);
        exit(1);
    }

    setup = xcb_get_setup(connection);
    iter = xcb_setup_roots_iterator(setup);
    while (scr-- > 0)
        xcb_screen_next(&iter);
    screen = iter.data;
}

void VulkanBase::initSwapchain()
{
    swapChain.initSurface(connection, window);
}

void VulkanBase::createCommandPool()
{
    VkCommandPoolCreateInfo cmdPoolInfo = {};
    cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.queueFamilyIndex = swapChain.queueNodeIndex;
    cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    VK_CHECK_RESULT(vkCreateCommandPool(device, &cmdPoolInfo, nullptr, &cmdPool));
}

void VulkanBase::setupSwapChain()
{
    swapChain.create(&width, &height, settings.vsync, settings.fullscreen);
}

void VulkanBase::createCommandBuffers()
{
    // Create one command buffer for each swap chain image and reuse for rendering
    drawCmdBuffers.resize(swapChain.imageCount);

    VkCommandBufferAllocateInfo cmdBufAllocateInfo =
            vks::initializers::commandBufferAllocateInfo(
                    cmdPool,
                    VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                    static_cast<uint32_t>(drawCmdBuffers.size()));

    VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &cmdBufAllocateInfo, drawCmdBuffers.data()));
}

void VulkanBase::createSynchronizationPrimitives()
{
    // Wait fences to sync command buffer access
    VkFenceCreateInfo fenceCreateInfo = vks::initializers::fenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);
    waitFences.resize(drawCmdBuffers.size());
    for (auto& fence : waitFences) {
        VK_CHECK_RESULT(vkCreateFence(device, &fenceCreateInfo, nullptr, &fence));
    }
}

/** @brief (Virtual) Setup default depth and stencil views */
void VulkanBase::setupDepthStencil(){
    VkImageCreateInfo imageCI{};
    imageCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCI.imageType = VK_IMAGE_TYPE_2D;
    imageCI.format = depthFormat;
    imageCI.extent = { width, height, 1 };
    imageCI.mipLevels = 1;
    imageCI.arrayLayers = 1;
    imageCI.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCI.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

    VK_CHECK_RESULT(vkCreateImage(device, &imageCI, nullptr, &depthStencil.image));
    VkMemoryRequirements memReqs{};
    vkGetImageMemoryRequirements(device, depthStencil.image, &memReqs);

    VkMemoryAllocateInfo memAllloc{};
    memAllloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAllloc.allocationSize = memReqs.size;
    memAllloc.memoryTypeIndex = vulkanDevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    VK_CHECK_RESULT(vkAllocateMemory(device, &memAllloc, nullptr, &depthStencil.mem));
    VK_CHECK_RESULT(vkBindImageMemory(device, depthStencil.image, depthStencil.mem, 0));

    VkImageViewCreateInfo imageViewCI{};
    imageViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCI.image = depthStencil.image;
    imageViewCI.format = depthFormat;
    imageViewCI.subresourceRange.baseMipLevel = 0;
    imageViewCI.subresourceRange.levelCount = 1;
    imageViewCI.subresourceRange.baseArrayLayer = 0;
    imageViewCI.subresourceRange.layerCount = 1;
    imageViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    // Stencil aspect should only be set on depth + stencil formats (VK_FORMAT_D16_UNORM_S8_UINT..VK_FORMAT_D32_SFLOAT_S8_UINT
    if (depthFormat >= VK_FORMAT_D16_UNORM_S8_UINT) {
        imageViewCI.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }
    VK_CHECK_RESULT(vkCreateImageView(device, &imageViewCI, nullptr, &depthStencil.view));
};

/** @brief (Virtual) Setup a default renderpass */
void VulkanBase::setupRenderPass(){

};

void VulkanBase::prepare()
{
    if (vulkanDevice->enableDebugMarkers) {
//        vks::debugmarker::setup(device);
    }
    initSwapchain();
    createCommandPool();
    setupSwapChain();
    createCommandBuffers();
    createSynchronizationPrimitives();
    setupDepthStencil();
    setupRenderPass();
//    createPipelineCache();
//    setupFrameBuffer();
//    settings.overlay = settings.overlay && (!benchmark.active);
//    if (settings.overlay) {
//        UIOverlay.device = vulkanDevice;
//        UIOverlay.queue = queue;
//        UIOverlay.shaders = {
//                loadShader(getShadersPath() + "base/uioverlay.vert.spv", VK_SHADER_STAGE_VERTEX_BIT),
//                loadShader(getShadersPath() + "base/uioverlay.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT),
//        };
//        UIOverlay.prepareResources();
//        UIOverlay.preparePipeline(pipelineCache, renderPass, swapChain.colorFormat, depthFormat);
//    }
}

void VulkanBase::render(){};
/** @brief (Virtual) Called when the camera view has changed */
void VulkanBase::viewChanged(){};
/** @brief (Virtual) Called after a key was pressed, can be used to do custom key handling */
void VulkanBase::keyPressed(uint32_t){};
/** @brief (Virtual) Called after the mouse cursor moved and before internal events (like camera rotation) is handled */
void VulkanBase::mouseMoved(double x, double y, bool &handled){};
/** @brief (Virtual) Called when the window has been resized, can be used by the sample application to recreate resources */
void VulkanBase::windowResized(){};
/** @brief (Virtual) Called when resources have been recreated that require a rebuild of the command buffers (e.g. frame buffer), to be implemented by the sample application */
void VulkanBase::buildCommandBuffers(){};
/** @brief (Virtual) Setup default framebuffers for all requested swapchain images */
void VulkanBase::setupFrameBuffer(){};
/** @brief (Virtual) Called after the physical device features have been read, can be used to set features to enable on the device */
void VulkanBase::getEnabledFeatures(){};
/** @brief (Virtual) Called after the physical device extensions have been read, can be used to enable extensions based on the supported extension listing*/
void VulkanBase::getEnabledExtensions(){};

