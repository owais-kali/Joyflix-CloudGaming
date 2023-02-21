#include <functional>
#include "Signalling_Handler.h"

Signalling_Handler::Signalling_Handler(int port, Signalling_Handler::DelegateOnOffer onOfferCallback,
                                       Signalling_Handler::DelegateOnAnswer onAnswerCallback,
                                       Signalling_Handler::DelegateOnICECandidate onIceCandidateCallback)
        : Port(port),
        onOfferDelegate(onOfferCallback), onAnswerDelegate(onAnswerCallback), onICECandidateDelegate(onIceCandidateCallback),
        signalling(std::bind(&Signalling_Handler::OnMessage, this, std::placeholders::_1))
{}

Signalling_Handler::~Signalling_Handler() {
    if (signalling_started) {
        StopSignalling();
    }
}

void Signalling_Handler::StartSignalling() {
    signalling.StartServer(Port);
    signalling_started = true;

    onOfferDelegate("This is Callback");
}

void Signalling_Handler::StopSignalling() {
    signalling_started = false;
    signalling.StopServer();
}

void Signalling_Handler::OnMessage(std::string msg) {
    printf("msg: %s\n",msg.c_str());
}

