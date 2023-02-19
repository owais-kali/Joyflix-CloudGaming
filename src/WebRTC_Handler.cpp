#include "Webrtc/API.h"
#include "WebRTC_Handler.h"
using namespace webrtc;

API* api;

WebRTC_Handler::WebRTC_Handler() {
    api = new API;
}

WebRTC_Handler::~WebRTC_Handler() {
    delete api;
}

void WebRTC_Handler::StartWebRTCApp(){
    api->ContextCreate();
    api->StartWebRTCServer();
}

void WebRTC_Handler::StopWebRTCApp(){
    api->ContextDestroy();
}