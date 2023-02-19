#pragma once
#include "Signalling/Signalling.h"

class Signalling_Handler {
public:
    using DelegateOnOffer = void (*)(std::string);
private:

    bool signalling_started;
    Signalling signalling;
    int Port;
    DelegateOnOffer onOfferDelegate;

    void OnMessage();
public:
    Signalling_Handler(int port, DelegateOnOffer onOfferCallback);
    ~Signalling_Handler();
    void StartSignalling();
    void StopSignalling();

    void RegisterOnOfferCallback(DelegateOnOffer callback);

    void RegisterOnAnswerCallback();

    void RegisterOnICECallback();
};



