#include "WebRTC_Handler.h"
#include "functional"
#include <csignal>
#include <future>
#include <unistd.h>

void WebRTC_Handler::OnGotRemoteDescription(webrtc::RTCSdpType type, std::string sdp) {
    printf("Got Remote SDP %s\n", sdp.c_str());
}
void WebRTC_Handler::OnGotRemoteIceCandidate(std::string ice, std::string sdpMLineIndex, int sdpMid) {
    printf("Got Remote ICE %s\n", ice.c_str());
}

WebRTC_Handler::WebRTC_Handler()
    : signalling_port(3001)
    , signalling_handler(std::make_unique<Signalling_Handler>(
          signalling_port,
          std::bind(&WebRTC_Handler::OnGotRemoteDescription, this, std::placeholders::_1, std::placeholders::_2),
          std::bind(&WebRTC_Handler::OnGotRemoteIceCandidate, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)))
{
    signalling_handler.get()->StartSignalling();
}

WebRTC_Handler::~WebRTC_Handler() {
    signalling_handler.get()->StopSignalling();
}

void WebRTC_Handler::StartWebRTCApp()
{
    pc.OnLocalDescription([](RTCSdpType sdpType, const char* desc, RTCErrorType errorType, const char* errMsg)
                          { printf("Got SDP \ntype:%d\n desc:\n%s\n", sdpType, desc); });

}

void WebRTC_Handler::StopWebRTCApp() { }

void WebRTC_Handler::SetLocalDescription(webrtc::RTCSdpType sdpType, char* sdp) { }
void WebRTC_Handler::SetRemoteDescription(RTCSessionDescription sdp) { pc.SetRemoteDescription(sdp); }

void WebRTC_Handler::AddICECandidate(char* candidate, char* sdpMLineIndex, int sdpMid) { }

