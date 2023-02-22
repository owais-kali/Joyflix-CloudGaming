#pragma once
#include "Webrtc/API.h"

class WebRTC_Handler {
public:
    WebRTC_Handler();
    ~WebRTC_Handler();
    void StartWebRTCApp();
    void StopWebRTCApp();
    void SetLocalDescription(webrtc::API::RTCSdpType sdpType, char* sdp);
    void SetRemoteDescription(webrtc::API::RTCSdpType sdpType, char* sdp);
};

