#include "WebRTC_Handler.h"

WebRTC_Handler::WebRTC_Handler(API::DelegateOnGotDescription onGotDescriptionCallback, API::DelegateOnGotICECandidate onGotIceCandidateCallback)
: api(onGotDescriptionCallback, onGotIceCandidateCallback)
{
    
}

WebRTC_Handler::~WebRTC_Handler() {
    
}

void WebRTC_Handler::StartWebRTCApp(){
    api.ContextCreate();
    api.StartWebRTCServer();
}

void WebRTC_Handler::StopWebRTCApp(){
    api.ContextDestroy();
}

void WebRTC_Handler::SetLocalDescription(API::RTCSdpType sdpType, char* sdp) {
    api.SetLocalDescription(sdpType, sdp);
}

void WebRTC_Handler::SetRemoteDescription(webrtc::API::RTCSdpType sdpType, char *sdp) {
    api.SetRemoteDescription(sdpType, sdp);
    if(sdpType == webrtc::API::RTCSdpType::Offer){
        api.CreateAnswer();
    }
}

void WebRTC_Handler::AddICECandidate(char *candidate, char *sdpMLineIndex, int sdpMid) {
    api.AddICECandidate(candidate, sdpMLineIndex, sdpMid);
}

