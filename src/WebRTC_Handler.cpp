#include "WebRTC_Handler.h"
#include "Webrtc/Types.h"
#include "functional"

WebRTC_Handler* WebRTC_Handler::s_instance;
WebRTC_Handler* WebRTC_Handler::GetInstance() { return s_instance; }

void WebRTC_Handler::OnGotRemoteDescription(webrtc::RTCSdpType type, std::string sdp)
{
    printf("Got Remote SDP %s\n", sdp.c_str());
    PC.SetRemoteDescription({ type, sdp.c_str() });
    PC.CreateAnswer({ true, true });
}

void WebRTC_Handler::OnGotLocalDescription(RTCSdpType sdpType, std::string sdp)
{
    PC.SetLocalDescription({ sdpType, sdp.c_str() });
    signalling_handler->SendSDP(sdpType, sdp);
    printf("Got Local SDP %s\n", sdp.c_str());
}

void WebRTC_Handler::OnGotRemoteIceCandidate(std::string candidate, std::string sdpMid, int sdpMLineIndex)
{
    PC.AddIceCandidate(const_cast<char*>(candidate.c_str()), const_cast<char*>(sdpMid.c_str()), sdpMLineIndex);
    printf("Got Remote ICE %s\n", candidate.c_str());
}

void WebRTC_Handler::OnGotLocalIceCandidate(std::string candidate, std::string sdpMid, int sdpMLineIndex) {
    signalling_handler->SendICE(const_cast<char*>(candidate.c_str()), const_cast<char*>(sdpMid.c_str()), sdpMLineIndex);
    printf("Got Local ICE %s\n", candidate.c_str());
}

WebRTC_Handler::WebRTC_Handler()
    : signalling_port(3001)
    , signalling_handler(std::make_unique<Signalling_Handler>(
          signalling_port,
          // OnGotDescriptionCallback
          [](webrtc::RTCSdpType sdpType, std::string sdp)
          { WebRTC_Handler::GetInstance()->OnGotRemoteDescription(sdpType, sdp); },
          // OnIceCandidateCallback
          [](std::string candidate, std::string sdpMLineIndex, int sdpMid)
          { WebRTC_Handler::GetInstance()->OnGotRemoteIceCandidate(candidate, sdpMLineIndex, sdpMid); }))
{
    if (s_instance == nullptr)
    {
        s_instance = this;
    }
    else
    {
        throw std::runtime_error("Only 1 instance of WebRTC_Handler is allowed!\n");
    }

    signalling_handler.get()->StartSignalling();

    PC.OnLocalDescription(
        [](RTCSdpType sdpType, const char* sdp)
        {
            std::string sdp_str = sdp;
            WebRTC_Handler::GetInstance()->OnGotLocalDescription(sdpType, sdp_str);
        });
    PC.OnIceCandidate(
        [](PeerConnectionObject* pco, const char* candidate, const char* sdpMid, const int sdpMLineIndex) {
            WebRTC_Handler::GetInstance()->OnGotLocalIceCandidate(candidate, sdpMid, sdpMLineIndex);
        });
}
void WebRTC_Handler::StartWebRTCApp() { }

WebRTC_Handler::~WebRTC_Handler() { signalling_handler.get()->StopSignalling(); }

void WebRTC_Handler::StopWebRTCApp() { }

void WebRTC_Handler::SetLocalDescription(webrtc::RTCSdpType sdpType, char* sdp) { }
void WebRTC_Handler::SetRemoteDescription(RTCSessionDescription sdp) { PC.SetRemoteDescription(sdp); }

void WebRTC_Handler::AddICECandidate(char* candidate, char* sdpMLineIndex, int sdpMid) { }
