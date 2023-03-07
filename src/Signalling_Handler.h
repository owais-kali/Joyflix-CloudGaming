#pragma once
#include "Signalling/Signalling.h"

namespace webrtc
{
enum class RTCSdpType;
}

using DelegateOnGotDescription = void (*)(webrtc::RTCSdpType, std::string);
using DelegateOnICECandidate = void (*)(std::string , std::string, int);

class Signalling_Handler
{
private:
    bool signalling_started;
    Signalling signalling;
    int Port;
    DelegateOnGotDescription onGotDescriptionDelegate;
    DelegateOnICECandidate onICECandidateDelegate;

    void OnMessage(std::string msg);

public:
    Signalling_Handler(
        int port, DelegateOnGotDescription onGotDescriptionCallback, DelegateOnICECandidate onIceCandidateCallback);
    ~Signalling_Handler();

    void RegisterOnConnectCallback(Signalling::DelegateOnConnect callback);
    void RegisterOnDisconnectCallback(Signalling::DelegateOnDisconnect callback);

    void StartSignalling();
    void StopSignalling();

    void SendSDP(webrtc::RTCSdpType type, std::string desc);
    void SendICE(char* candidate, char* sdpMid, int sdpMlineIndex);
};
