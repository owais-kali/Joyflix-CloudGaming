#pragma once
#include "Webrtc/API.h"

using namespace webrtc;
class WebRTC_Handler {
private:
    API api;
public:
    WebRTC_Handler(API::DelegateOnGotDescription onGotDescriptionCallback, API::DelegateOnGotICECandidate onGotIceCandidateCallback);
    ~WebRTC_Handler();
    void StartWebRTCApp();
    void StopWebRTCApp();
    void SetLocalDescription(webrtc::API::RTCSdpType sdpType, char* sdp);
    void SetRemoteDescription(webrtc::API::RTCSdpType sdpType, char* sdp);
};

