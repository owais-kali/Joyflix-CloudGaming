#include "WebRTC_Handler.h"
#include <future>
#include <unistd.h>
#include <csignal>

std::future<void> AddICECandidateFunc;
volatile sig_atomic_t StopAddICECandidateFunc;

WebRTC_Handler::WebRTC_Handler(API::DelegateOnGotDescription onGotDescriptionCallback, API::DelegateOnGotICECandidate onGotIceCandidateCallback)
: api(onGotDescriptionCallback, onGotIceCandidateCallback)
{
    
}

WebRTC_Handler::~WebRTC_Handler() {
    if(AddICECandidateFunc.valid()){
        StopAddICECandidateFunc = 1;
        AddICECandidateFunc.wait();
    }
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
    std::string candidate_ = candidate;
    std::string sdpMLineIndex_ = sdpMLineIndex;
    AddICECandidateFunc = std::async([]( API* api, std::string candidate, std::string sdpMLineIndex, int sdpMid){
        while (!StopAddICECandidateFunc) {
            API::SignalingState state = api->GetSignallingState();
            switch (state) {
                case webrtc::API::kStable:
                    printf("kStable\n");
                    api->AddICECandidate(const_cast<char *>(candidate.c_str()),
                                         const_cast<char *>(sdpMLineIndex.c_str()), sdpMid);
                    return;
            }
            sleep(1);
        }
    }, &api, candidate_, sdpMLineIndex_, sdpMid);
}

