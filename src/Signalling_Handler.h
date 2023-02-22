#pragma once
#include "Signalling/Signalling.h"
#include "Webrtc/API.h"

class Signalling_Handler {
public:
    using DelegateOnGotDescription = void (*)(webrtc::API::RTCSdpType, std::string);
    using DelegateOnICECandidate = void (*)(std::string, std::string, int);
private:
    bool signalling_started;
    Signalling signalling;
    int Port;
    DelegateOnGotDescription onOnGotDescriptionDelegate;
    DelegateOnICECandidate onICECandidateDelegate;

    void OnMessage(std::string msg);
public:
    Signalling_Handler(int port, DelegateOnGotDescription onGotDescriptionCallback,
                       DelegateOnICECandidate onIceCandidateCallback);
    ~Signalling_Handler();
    void StartSignalling();
    void StopSignalling();

    void SendSDP(webrtc::API::RTCSdpType type ,std::string desc);
};



