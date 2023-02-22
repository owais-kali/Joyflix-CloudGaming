#include <functional>
#include <nlohmann/json.hpp>
#include "Signalling_Handler.h"

using namespace nlohmann;

Signalling_Handler::Signalling_Handler(int port, Signalling_Handler::DelegateOnGotDescription onGotDescriptionCallback,
                                       Signalling_Handler::DelegateOnICECandidate onIceCandidateCallback)
        : Port(port),
        onGotDescriptionDelegate(onGotDescriptionCallback), onICECandidateDelegate(onIceCandidateCallback),
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
}

void Signalling_Handler::StopSignalling() {
    signalling_started = false;
    signalling.StopServer();
}

void Signalling_Handler::OnMessage(std::string msg) {
    json data = json::parse(msg);
    std::string type = data["type"].get<std::string>();

    if(type=="offer"){
        std::string sdp = data["data"]["sdp"].get<std::string>();
        onGotDescriptionDelegate(webrtc::API::RTCSdpType::Offer ,sdp);
        return;
    }
    if(type=="candidate"){
        std::string candidate = data["data"]["candidate"].get<std::string>();
        std::string sdpMLineIndex = data["data"]["sdpMLineIndex"].get<std::string>();
        int sdpMid = data["data"]["sdpMid"].get<int>();
        onICECandidateDelegate(candidate, sdpMLineIndex, sdpMid);
        return;
    }
}

void Signalling_Handler::SendSDP(webrtc::API::RTCSdpType type, std::string desc) {

}

