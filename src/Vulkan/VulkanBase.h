#pragma once

#include <xcb/xcb.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <array>
#include <unordered_map>
#include <numeric>
#include <ctime>
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <sys/stat.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <numeric>
#include <array>

#include "vulkan/vulkan.h"

#include "CommandLineParser.hpp"
#include "VulkanTools.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanDebug.h"
#include "benchmark.hpp"
#include "camera.hpp"

#include "keycodes.hpp"

class VulkanBase {
private:
    std::string getWindowTitle();
    uint32_t destWidth;
    uint32_t destHeight;
    bool resizing = false;

    void windowResize();
    void handleMouseMove(int32_t x, int32_t y);
    void nextFrame();
    void updateOverlay();
    void createPipelineCache();
    void initSwapchain();
    void createCommandPool();
    void setupSwapChain();
    void createCommandBuffers();
    void createSynchronizationPrimitives();

    std::string shaderDir = "glsl";

public:
    bool prepared = false;
    bool resized = false;
    bool viewUpdated = false;
    uint32_t width = 1280;
    uint32_t height = 720;

    CommandLineParser commandLineParser;

    /** @brief Last frame time measured using a high performance timer (if available) */
    float frameTimer = 1.0f;

    vks::Benchmark benchmark;

    /** @brief Encapsulated physical and logical vulkan device */
    vks::VulkanDevice *vulkanDevice;
    /** @brief Example settings that can be changed e.g. by command line arguments */
    struct Settings {
        /** @brief Activates validation layers (and message output) when set to true */
        bool validation = true;
        /** @brief Set to true if fullscreen mode has been requested via command line */
        bool fullscreen = false;
        /** @brief Set to true if v-sync will be forced for the swapchain */
        bool vsync = false;
        /** @brief Enable UI overlay */
        bool overlay = true;
    } settings;

    VkClearColorValue defaultClearColor = { { 0.025f, 0.025f, 0.025f, 1.0f } };

    static std::vector<const char*> args;
    // Defines a frame rate independent timer value clamped from -1.0...1.0
    // For use in animations, rotations, etc.
    float timer = 0.0f;
    // Multiplier for speeding up (or slowing down) the global timer
    float timerSpeed = 0.25f;
    bool paused = false;

    Camera camera;
    glm::vec2 mousePos;

    std::string title = "Vulkan Example";
    std::string name = "vulkanExample";
    uint32_t apiVersion = VK_API_VERSION_1_0;

    struct {
        VkImage image;
        VkDeviceMemory mem;
        VkImageView view;
    } depthStencil;

    struct {
        glm::vec2 axisLeft = glm::vec2(0.0f);
        glm::vec2 axisRight = glm::vec2(0.0f);
    } gamePadState;

    struct {
        bool left = false;
        bool right = false;
        bool middle = false;
    } mouseButtons;
protected:

    // Frame counter to display fps
    uint32_t frameCounter = 0;
    uint32_t lastFPS = 0;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTimestamp, tPrevEnd;
    // Vulkan instance, stores all per-application states
    VkInstance instance;
    std::vector<std::string> supportedInstanceExtensions;
    // Physical device (GPU) that Vulkan will use
    VkPhysicalDevice physicalDevice;
    // Stores physical device properties (for e.g. checking device limits)
    VkPhysicalDeviceProperties deviceProperties;
    // Stores the features available on the selected physical device (for e.g. checking if a feature is available)
    VkPhysicalDeviceFeatures deviceFeatures;
    // Stores all available memory (type) properties for the physical device
    VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
    /** @brief Set of physical device features to be enabled for this example (must be set in the derived constructor) */
    VkPhysicalDeviceFeatures enabledFeatures{};
    /** @brief Set of device extensions to be enabled for this example (must be set in the derived constructor) */
    std::vector<const char*> enabledDeviceExtensions;
    std::vector<const char*> enabledInstanceExtensions;
    /** @brief Optional pNext structure for passing extension structures to device creation */
    void* deviceCreatepNextChain = nullptr;
    /** @brief Logical device, application's view of the physical device (GPU) */
    VkDevice device;
    // Handle to the device graphics queue that command buffers are submitted to
    VkQueue queue;
    // Depth buffer format (selected during Vulkan initialization)
    VkFormat depthFormat;
    // Command buffer pool
    VkCommandPool cmdPool;
    /** @brief Pipeline stages used to wait at for graphics queue submissions */
    VkPipelineStageFlags submitPipelineStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    // Contains command buffers and semaphores to be presented to the queue
    VkSubmitInfo submitInfo;
    // Command buffers used for rendering
    std::vector<VkCommandBuffer> drawCmdBuffers;
    // Global render pass for frame buffer writes
    VkRenderPass renderPass = VK_NULL_HANDLE;
    // List of available frame buffers (same as number of swap chain images)
    std::vector<VkFramebuffer>frameBuffers;
    // Active frame buffer index
    uint32_t currentBuffer = 0;
    // Descriptor set pool
    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    // List of shader modules created (stored for cleanup)
    std::vector<VkShaderModule> shaderModules;
    // Pipeline cache object
    VkPipelineCache pipelineCache;
    // Wraps the swap chain to present images (framebuffers) to the windowing system

    VulkanSwapChain swapChain;

    // Synchronization semaphores
    struct {
        // Swap chain image presentation
        VkSemaphore presentComplete;
        // Command buffer submission and execution
        VkSemaphore renderComplete;
    } semaphores;
    std::vector<VkFence> waitFences;

public:
    bool quit = false;
    xcb_connection_t *connection;
    xcb_screen_t *screen;
    xcb_window_t window;
    xcb_intern_atom_reply_t *atom_wm_delete_window;

    VulkanBase(bool enableValidation);
    virtual ~VulkanBase();
    /** @brief Setup the vulkan instance, enable required extensions and connect to the physical device (GPU) */
    bool initVulkan();
    xcb_window_t setupWindow();
    void initxcbConnection();
    void handleEvent(const xcb_generic_event_t *event);

    /** @brief (Virtual) Creates the application wide Vulkan instance */
    virtual VkResult createInstance(bool enableValidation);
    /** @brief (Pure virtual) Render function to be implemented by the sample application */
    virtual void render() = 0;
    /** @brief (Virtual) Called when the camera view has changed */
    virtual void viewChanged();
    /** @brief (Virtual) Called after a key was pressed, can be used to do custom key handling */
    virtual void keyPressed(uint32_t);
    /** @brief (Virtual) Called after the mouse cursor moved and before internal events (like camera rotation) is handled */
    virtual void mouseMoved(double x, double y, bool &handled);
    /** @brief (Virtual) Called when the window has been resized, can be used by the sample application to recreate resources */
    virtual void windowResized();
    /** @brief (Virtual) Called when resources have been recreated that require a rebuild of the command buffers (e.g. frame buffer), to be implemented by the sample application */
    virtual void buildCommandBuffers();
    /** @brief (Virtual) Setup default depth and stencil views */
    virtual void setupDepthStencil();
    /** @brief (Virtual) Setup default framebuffers for all requested swapchain images */
    virtual void setupFrameBuffer();
    /** @brief (Virtual) Setup a default renderpass */
    virtual void setupRenderPass();
    /** @brief (Virtual) Called after the physical device features have been read, can be used to set features to enable on the device */
    virtual void getEnabledFeatures();
    /** @brief (Virtual) Called after the physical device extensions have been read, can be used to enable extensions based on the supported extension listing*/
    virtual void getEnabledExtensions();

    /** @brief Prepares all Vulkan resources and functions required to run the sample */
    virtual void prepare();

    /** @brief Entry point for the main render loop */
    void renderLoop();
private:
    std::string shader_path;
public:
    void setShaderPath(std::string &path){shader_path = path;};
    std::string getShadersPath(){return shader_path;};
};
