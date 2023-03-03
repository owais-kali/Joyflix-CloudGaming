#pragma once
#include "Signalling/Signalling.h"
#include "Webrtc/API.h"

class Signalling_Handler {
public:
    typedef std::function<void (webrtc::RTCSdpType, std::string)> DelegateOnGotDescription;
    typedef std::function<void (std::string, std::string, int)> DelegateOnICECandidate;

private:
    bool signalling_started;
    Signalling signalling;
    int Port;
    DelegateOnGotDescription onGotDescriptionDelegate;
    DelegateOnICECandidate onICECandidateDelegate;

    void OnMessage(std::string msg);
public:
    Signalling_Handler(int port, DelegateOnGotDescription onGotDescriptionCallback,
                       DelegateOnICECandidate onIceCandidateCallback);
    ~Signalling_Handler();
    void StartSignalling();
    void StopSignalling();

    void SendSDP(webrtc::RTCSdpType type ,std::string desc);
    void SendICE(char* candidate, char* sdpMid, int sdpMlineIndex);
};



