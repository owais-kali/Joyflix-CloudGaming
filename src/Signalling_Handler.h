#pragma once
#include "Signalling/Signalling.h"

class Signalling_Handler {
public:
    using DelegateOnOffer = void (*)(std::string);
    using DelegateOnAnswer = void (*)(std::string);
    using DelegateOnICECandidate = void (*)(std::string, std::string, int);
private:
    bool signalling_started;
    Signalling signalling;
    int Port;
    DelegateOnOffer onOfferDelegate;
    DelegateOnAnswer onAnswerDelegate;
    DelegateOnICECandidate onICECandidateDelegate;

    void OnMessage(std::string msg);
public:
    Signalling_Handler(int port, DelegateOnOffer onOfferCallback,
                       DelegateOnAnswer onAnswerCallback,
                       DelegateOnICECandidate onIceCandidateCallback);
    ~Signalling_Handler();
    void StartSignalling();
    void StopSignalling();
};



