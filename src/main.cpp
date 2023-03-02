#include "stdio.h"
#include "Vulkan/VulkanBase.h"
#include "Cuda/CudaContext.h"
#include "Signalling_Handler.h"
#include "WebRTC_Handler.h"
#include "iostream"
#include <chrono>
#include <thread>
#include <signal.h>

#include "VulkanApp.h"
#include "Webrtc/API.h"

#define USE_STAGING true

VulkanApp *vulkanApp;
CudaContext *cuda_ctx;

void StartCudaApp() {
    cuda_ctx = new CudaContext();
    cuda_ctx->Init();
}

void StartVulkanApp() {
    vulkanApp = new VulkanApp();
    vulkanApp->initVulkan(static_cast<void *>(cuda_ctx->GetUUID().bytes));
    vulkanApp->setupWindow();
    vulkanApp->prepare();
    vulkanApp->renderLoop();
}

WebRTC_Handler *webRtcHandler;
Signalling_Handler *signalling_handler;

void OnGotRemoteDescription(webrtc::API::RTCSdpType type, std::string offer) {
    std::cout << "Got Remote Description: " << std::endl;
    webRtcHandler->SetRemoteDescription(webrtc::API::RTCSdpType::Offer, const_cast<char *>(offer.c_str()));
}

void OnGotLocalDescription(webrtc::API::RTCSdpType type, char *desc) {
    std::cout << "Got Local Description: " << std::endl;
    signalling_handler->SendSDP(type, desc);
    webRtcHandler->SetLocalDescription(type, desc);
}

void OnGotLocalIceCandidate(char *candidate, char *sdpMLineIndex, int sdpMid) {
    std::cout << "Got Local ICE: " << std::endl;
    signalling_handler->SendICE(candidate, sdpMLineIndex, sdpMid);

}

void OnGotRemoteIceCandidate(std::string ice, std::string sdpMLineIndex, int sdpMid) {
    webRtcHandler->AddICECandidate((char *) ice.c_str(), (char *) sdpMLineIndex.c_str(), sdpMid);
    std::cout << "Add Remote ICE: " << ice << std::endl;
}

void StartSignallingServer() {
    signalling_handler = new Signalling_Handler(3001, OnGotRemoteDescription, OnGotRemoteIceCandidate);
    signalling_handler->StartSignalling();
}

void StopSignallingServer() {
    signalling_handler->StopSignalling();
    delete signalling_handler;
}

volatile sig_atomic_t stop;

void sigterm_callback(int signum) {
    stop = 1;
}

int main(int argc, char *argv[]) {
    signal(SIGTERM, sigterm_callback);

    StartSignallingServer();

    webRtcHandler = new WebRTC_Handler(OnGotLocalDescription, OnGotLocalIceCandidate);
    webRtcHandler->StartWebRTCApp();

    while (!stop);

    StopSignallingServer();
    webRtcHandler->StopWebRTCApp();

    return EXIT_SUCCESS;
    for (size_t i = 0; i < argc; i++) { VulkanApp::args.push_back(argv[i]); };
    StartCudaApp();
    StartVulkanApp();

    delete vulkanApp;
    delete cuda_ctx;
}