#include "Webrtc/API.h"
#include "WebRTC_Handler.h"
using namespace webrtc;

#define _Context void*

API* api;
_Context ctx;

WebRTC_Handler::WebRTC_Handler() {
    api = new API;
}

WebRTC_Handler::~WebRTC_Handler() {
    delete api;
}

void WebRTC_Handler::StartWebRTCApp(){
    ctx = api->ContextCreate();
}

void WebRTC_Handler::StopWebRTCApp(){
    api->ContextDestroy(ctx);
}