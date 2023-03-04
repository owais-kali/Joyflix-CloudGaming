#include "Cuda/CudaContext.h"
#include "Signalling_Handler.h"
#include "Vulkan/VulkanBase.h"
#include "WebRTC_Handler.h"
#include "iostream"
#include "stdio.h"
#include <chrono>
#include <signal.h>
#include <thread>

#include "VulkanApp.h"
#include "Webrtc/API.h"

#define USE_STAGING true

VulkanApp* vulkanApp;
CudaContext* cuda_ctx;

void StartCudaApp()
{
    cuda_ctx = new CudaContext();
    cuda_ctx->Init();
}

void StartVulkanApp()
{
    vulkanApp = new VulkanApp();
    vulkanApp->initVulkan(static_cast<void*>(cuda_ctx->GetUUID().bytes));
    vulkanApp->setupWindow();
    vulkanApp->prepare();
    vulkanApp->renderLoop();
}

WebRTC_Handler* webRtcHandler;

volatile sig_atomic_t stop;

void sigterm_callback(int signum) { stop = 1; }

int main(int argc, char* argv[])
{
    signal(SIGTERM, sigterm_callback);
    signal(-1, sigterm_callback);

    webRtcHandler = new WebRTC_Handler;
    webRtcHandler->StartWebRTCApp();

    while (!stop);

    webRtcHandler->StopWebRTCApp();
    delete webRtcHandler;

    return EXIT_SUCCESS;
    for (size_t i = 0; i < argc; i++)
    {
        VulkanApp::args.push_back(argv[i]);
    };
    StartCudaApp();
    StartVulkanApp();

    delete vulkanApp;
    delete cuda_ctx;
}