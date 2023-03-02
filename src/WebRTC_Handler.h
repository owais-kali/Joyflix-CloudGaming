#pragma once
#include "Webrtc/API.h"

using namespace webrtc;

class WebRTC_Handler {
private:
    API api;
    PeerConnectionObject* pc;
public:
    WebRTC_Handler();
    ~WebRTC_Handler();
    void StartWebRTCApp();
    void StopWebRTCApp();
    void SetLocalDescription(API::RTCSdpType sdpType, char* sdp);
    void SetRemoteDescription(API::RTCSdpType sdpType, char* sdp);
    void AddICECandidate(char* candidate, char* sdpMLineIndex, int sdpMid);
};

