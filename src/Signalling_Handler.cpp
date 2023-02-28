#include <functional>
#include <nlohmann/json.hpp>
#include "Signalling_Handler.h"

using namespace nlohmann;

Signalling_Handler::Signalling_Handler(int port, Signalling_Handler::DelegateOnGotDescription onGotDescriptionCallback,
                                       Signalling_Handler::DelegateOnICECandidate onIceCandidateCallback)
        : Port(port),
          onGotDescriptionDelegate(onGotDescriptionCallback), onICECandidateDelegate(onIceCandidateCallback),
          signalling(std::bind(&Signalling_Handler::OnMessage, this, std::placeholders::_1)) {}

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
    printf("OnMessage\n%s\n", msg.c_str());
    json data = json::parse(msg);
    std::string type = data["type"].get<std::string>();

    if (type == "offer") {
        std::string sdp = data["data"]["sdp"].get<std::string>();
        onGotDescriptionDelegate(webrtc::API::RTCSdpType::Offer, sdp);
        return;
    }
    if (type == "candidate") {
        std::string candidate = data["data"]["candidate"].get<std::string>();
        std::string sdpMLineIndex = data["data"]["sdpMLineIndex"].get<std::string>();
        int sdpMid = data["data"]["sdpMid"].get<int>();
        onICECandidateDelegate(candidate, sdpMLineIndex, sdpMid);
        return;
    }
}

void Signalling_Handler::SendSDP(webrtc::API::RTCSdpType type, std::string desc) {
    switch (type) {
        case webrtc::API::RTCSdpType::Answer:
            json data = json::parse(R"(
                {
                  "data": {
                    "connectionId": "server",
                    "sdp": ""
                  },
                  "from": "f67a7cf5-9950-4ffc-b97c-3370df518964",
                  "type": "answer"
                }
            )");
            data["data"]["sdp"] = desc;
            printf("SendSDP: \n%s\n", data.dump().c_str());

            signalling.SendMessage(data.dump());
            break;
    }
}

void Signalling_Handler::SendICE(char* candidate, char* sdpMlineIndex, int sdpMid){
    json data = json::parse(R"(
                {
                  "type": "candidate",
                  "from": "9dc9690c-183f-404a-bb7e-3e57f5a59566",
                  "data": {
                    "candidate": "",
                    "sdpMLineIndex": "",
                    "sdpMid": 0,
                    "connectionId": "9dc9690c-183f-404a-bb7e-3e57f5a59566"
                  }
                }
            )");
    data["data"]["candidate"] = candidate;
    data["data"]["sdpMlineIndex"] = sdpMlineIndex;
    data["data"]["sdpMid"] = sdpMid;
    printf("SendICE: \n%s\n", data.dump().c_str());

    signalling.SendMessage(data.dump());
}

