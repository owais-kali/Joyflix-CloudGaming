
#include "Signalling_Handler.h"

Signalling_Handler::Signalling_Handler(int port, DelegateOnOffer onOfferCallback)
        : Port(port), onOfferDelegate(onOfferCallback) {

}

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

void Signalling_Handler::RegisterOnOfferCallback(DelegateOnOffer callback) {

}

void Signalling_Handler::RegisterOnAnswerCallback() {

}

void Signalling_Handler::RegisterOnICECallback() {

}

void Signalling_Handler::OnMessage() {

}
