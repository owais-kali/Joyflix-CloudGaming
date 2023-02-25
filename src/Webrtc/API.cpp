#include "API.h"
#include "Context.h"
#include "WebRTCPlugin.h"
#include "Logger.h"

using namespace webrtc;

API* api;
WebRTCPlugin* plugin;
Context* ctx;
PeerConnectionObject* pco;

API::API(DelegateOnGotDescription onGotDescriptionCallback, DelegateOnGotICECandidate onGotICECandidateCallback)
: GotDescriptionCallback(onGotDescriptionCallback), GotICECandidateCallback(onGotICECandidateCallback)
{
    api = this;
    plugin = new WebRTCPlugin;
    DebugLog("Plugin Created!");
}

API::~API() {
    DebugLog("Plugin Destroyed!");
    delete plugin;
}

void* API::ContextCreate() {
    DebugLog("Context Created!");
    ctx = new Context;
    return ctx;
}

void API::ContextDestroy() {
    delete ctx;
    DebugLog("Context Destroyed!");
}

void GotSDPCallback(PeerConnectionObject* pco, RTCSdpType type, const char* sdp) {
    switch (type) {
        case RTCSdpType::Offer: {
            RTCSessionDescription desc = {};
            desc.sdp = sdp;
            desc.type = type;
            std::string error;
            api->GotDescriptionCallback(webrtc::API::RTCSdpType::Offer, const_cast<char*>(sdp));
            LogPrint("Offer SDP: \n%s", sdp);
        } break;

        case RTCSdpType::Answer: {
            RTCSessionDescription desc = {};
            desc.sdp = sdp;
            desc.type = type;
            std::string error;

            api->GotDescriptionCallback(webrtc::API::RTCSdpType::Answer, const_cast<char*>(sdp));
            LogPrint("Answer SDP: \n%s", sdp);
        } break;
        default:
            // TODO:
            break;
    }
}

void OnIceCandidate(PeerConnectionObject* pco,
                    const char* candidate,
                    const char* sdpMid,
                    const int sdpMlineIndex) {
    api->GotICECandidateCallback((char*)candidate, (char*)sdpMid, sdpMlineIndex);
}

void CreateOffer(PeerConnectionObject* pco) {
    const RTCOfferAnswerOptions options{false, true};
    plugin->PeerConnectionCreateOffer(pco, &options);
}

void API::StartWebRTCServer() {
    pco = plugin->ContextCreatePeerConnection(ctx);
    plugin->PeerConnectionRegisterCallbackCreateSD(pco, GotSDPCallback, nullptr);
    plugin->PeerConnectionRegisterOnIceCandidate(pco, OnIceCandidate);
    plugin->AddTracks(ctx);
}

void API::SetLocalDescription(API::RTCSdpType type, char *sdp) {
    switch (type) {
        case RTCSdpType::Offer:
            RTCSessionDescription desc = {webrtc::RTCSdpType::Offer, sdp};
            std::string err;
            if(plugin->PeerConnectionSetLocalDescription(ctx, pco, &desc, err)!=RTCErrorType::NONE)
            {
                DebugLog("%s",err.c_str());
            }
            break;
    }
}

void API::SetRemoteDescription(API::RTCSdpType type, char *sdp) {
    switch (type) {
        case RTCSdpType::Offer:
            RTCSessionDescription desc = {webrtc::RTCSdpType::Offer, sdp};
            std::string err;
            if(plugin->PeerConnectionSetRemoteDescription(ctx, pco, &desc, err)!=RTCErrorType::NONE)
            {
                DebugLog("%s",err.c_str());
            }
            break;
    }
}

void API::AddICECandidate(char *candidate, char *sdpMLineIndex, int sdpMid) {
    RTCIceCandidateInit options = {candidate, sdpMLineIndex, sdpMid};
    plugin->PeerConnectionAddIceCandidate(pco,candidate, sdpMLineIndex, sdpMid);
}

void API::CreateAnswer() {
    RTCOfferAnswerOptions options = {true, true};
    plugin->PeerConnectionCreateAnswer(pco, &options);
}

API::SignalingState API::GetSignallingState() {
    PeerConnectionInterface::SignalingState state =  plugin->PeerConnectionSignalingState(pco);
    switch (state) {
        case PeerConnectionInterface::kHaveLocalOffer:
            return API::SignalingState::kHaveLocalOffer;
        case PeerConnectionInterface::kHaveRemoteOffer:
            return API::SignalingState::kHaveRemoteOffer;
        case PeerConnectionInterface::kStable:
            return API::SignalingState::kStable;
        case PeerConnectionInterface::kClosed:
            return API::SignalingState::kClosed;
        case PeerConnectionInterface::kHaveLocalPrAnswer:
            return API::SignalingState::kHaveLocalPrAnswer;
        case PeerConnectionInterface::kHaveRemotePrAnswer:
            return API::SignalingState::kHaveRemotePrAnswer;
    }
}

