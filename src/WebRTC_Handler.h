#pragma once
#include "Webrtc/API.h"

using namespace joyflix::webrtc;

class WebRTC_Handler {
private:
    API api;

public:
    WebRTC_Handler(API::DelegateOnGotDescription onGotDescriptionCallback, API::DelegateOnGotICECandidate onGotIceCandidateCallback);
    ~WebRTC_Handler();
    void StartWebRTCApp();
    void StopWebRTCApp();
    void SetLocalDescription(API::RTCSdpType sdpType, char* sdp);
    void SetRemoteDescription(API::RTCSdpType sdpType, char* sdp);
    void AddICECandidate(char* candidate, char* sdpMLineIndex, int sdpMid);
};

