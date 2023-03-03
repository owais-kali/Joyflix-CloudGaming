#pragma once
#include "Webrtc/API.h"
#include "Signalling_Handler.h"

using namespace webrtc;

class WebRTC_Handler {
private:
    const int signalling_port;
    std::unique_ptr<Signalling_Handler> signalling_handler;

    API api;
    RTCPeerConnection pc;

    void OnGotRemoteDescription(webrtc::RTCSdpType type, std::string sdp);
    void OnGotRemoteIceCandidate(std::string ice, std::string sdpMLineIndex, int sdpMid);

public:
    WebRTC_Handler();
    ~WebRTC_Handler();
    void StartWebRTCApp();
    void StopWebRTCApp();
    void SetLocalDescription(webrtc::RTCSdpType sdpType, char* sdp);
    void SetRemoteDescription(RTCSessionDescription sdp);
    void AddICECandidate(char* candidate, char* sdpMLineIndex, int sdpMid);
};

