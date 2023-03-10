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
    std::unique_ptr<RTCPeerConnection> PC;

    void OnClientConnect();
    void OnClientDisConnect();

    void OnGotRemoteDescription(webrtc::RTCSdpType type, std::string sdp);
    void OnGotLocalDescription(RTCSdpType sdpType, std::string sdp);

    void OnGotRemoteIceCandidate(std::string candidate, std::string sdpMid, int sdpMLineIndex);
    void OnGotLocalIceCandidate(std::string candidate, std::string sdpMid, int sdpMLineIndex);

public:
    static WebRTC_Handler* GetInstance();

    WebRTC_Handler();
    ~WebRTC_Handler();
    void StartWebRTCApp();
    void StopWebRTCApp();
};
