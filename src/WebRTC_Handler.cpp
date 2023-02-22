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

void WebRTC_Handler::SetLocalDescription(API::RTCSdpType sdpType, char* sdp) {
    api->SetLocalDescription(sdpType, sdp);
}

void WebRTC_Handler::SetRemoteDescription(webrtc::API::RTCSdpType sdpType, char *sdp) {
    api->SetRemoteDescription(sdpType, sdp);
    if(sdpType == webrtc::API::RTCSdpType::Offer){
        api->CreateAnswer();
    }
}

