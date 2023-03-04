#pragma once
#include "Signalling_Handler.h"
#include "Webrtc/API.h"

using namespace webrtc;

class WebRTC_Handler
{
private:
    static WebRTC_Handler* s_instance;

    const int signalling_port;
    std::unique_ptr<Signalling_Handler> signalling_handler;

    API api;
    RTCPeerConnection PC;

    void OnGotRemoteDescription(webrtc::RTCSdpType type, std::string sdp);
    void OnGotRemoteIceCandidate(std::string candidate, std::string sdpMLineIndex, int sdpMid);
    void OnGotLocalDescription(RTCSdpType sdpType, std::string sdp);

public:
    static WebRTC_Handler* GetInstance();

    WebRTC_Handler();
    ~WebRTC_Handler();
    void StartWebRTCApp();
    void StopWebRTCApp();
    void SetLocalDescription(webrtc::RTCSdpType sdpType, char* sdp);
    void SetRemoteDescription(RTCSessionDescription sdp);
    void AddICECandidate(char* candidate, char* sdpMLineIndex, int sdpMid);
};
