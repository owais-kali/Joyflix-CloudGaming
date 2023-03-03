#include "WebRTC_Handler.h"
#include <csignal>
#include <future>
#include <unistd.h>

std::future<void> AddICECandidateFunc;
volatile sig_atomic_t StopAddICECandidateFunc;

WebRTC_Handler::WebRTC_Handler() {
    signalling_handler = std::make_unique<Signalling_Handler>(3001, nullptr, nullptr);
}

WebRTC_Handler::~WebRTC_Handler()
{
    if (AddICECandidateFunc.valid())
    {
        StopAddICECandidateFunc = 1;
        AddICECandidateFunc.wait();
    }
}

void WebRTC_Handler::StartWebRTCApp()
{
    pc.OnLocalDescription([](RTCSdpType sdpType, const char* desc, RTCErrorType errorType, const char* errMsg)
                          { printf("Got SDP \ntype:%d\n desc:\n%s\n", sdpType, desc); });
    pc.CreateOffer(webrtc::RTCOfferAnswerOptions { true, true });
}

void WebRTC_Handler::StopWebRTCApp() { }

void WebRTC_Handler::SetLocalDescription(API::RTCSdpType sdpType, char* sdp) { }

void WebRTC_Handler::SetRemoteDescription(RTCSessionDescription sdp) { pc.SetRemoteDescription(sdp); }

void WebRTC_Handler::AddICECandidate(char* candidate, char* sdpMLineIndex, int sdpMid)
{
    std::string candidate_ = candidate;
    std::string sdpMLineIndex_ = sdpMLineIndex;
    // TODO: Add remote description before adding ICE candidate
    return;
    AddICECandidateFunc = std::async(
        [](API* api, std::string candidate, std::string sdpMLineIndex, int sdpMid)
        {
            while (!StopAddICECandidateFunc)
            {
                API::SignalingState state; //= api->GetSignallingState();
                switch (state)
                {
                case API::kHaveRemoteOffer:
                    printf("kStable\n");
                    api->AddICECandidate(
                        const_cast<char*>(candidate.c_str()), const_cast<char*>(sdpMLineIndex.c_str()), sdpMid);
                    return;
                }
                sleep(1);
            }
        },
        &api,
        candidate_,
        sdpMLineIndex_,
        sdpMid);
}
