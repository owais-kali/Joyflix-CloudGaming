#pragma once
#include "Webrtc/API.h"
#include "Signalling_Handler.h"

using namespace webrtc;

class WebRTC_Handler {
private:
    std::unique_ptr<Signalling_Handler> signalling_handler;
    API api;
    RTCPeerConnection pc;
public:
    WebRTC_Handler();
    ~WebRTC_Handler();
    void StartWebRTCApp();
    void StopWebRTCApp();
    void SetLocalDescription(API::RTCSdpType sdpType, char* sdp);
    void SetRemoteDescription(RTCSessionDescription sdp);
    void AddICECandidate(char* candidate, char* sdpMLineIndex, int sdpMid);
};

