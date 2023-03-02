#include "API.h"
#include "Context.h"
#include "Logger.h"
#include "WebRTCPlugin.h"

using namespace webrtc;

static API* api;
static WebRTCPlugin* plugin;

void OnSessionDescriptionObserverCallback(
    PeerConnectionObject* pco,
    CreateSessionDescriptionObserver* csdo,
    RTCSdpType rtcSdpType,
    const char* desc,
    RTCErrorType errorType,
    const char* errorMsg)
{
    DebugLog("OnSessionDescriptionObserverCallback: desc:\n%s", desc);
}

RTCPeerConnection::RTCPeerConnection() { }
RTCPeerConnection::RTCPeerConnection(const char* config) { }

API::API()
{
    if (api != nullptr)
    {
        // Only 1 instance of API is allowed
        DebugError("Only 1 instance of API is allowed");
        return;
    }
    if (plugin != nullptr)
    {
        // Only 1 instance of Webrtc Plugin is allowed
        DebugError("Only 1 instance of Plugin is allowed");
        return;
    }

    api = this;
    plugin = new WebRTCPlugin;
    plugin->CreateSessionDescriptionObserverRegisterCallback(OnSessionDescriptionObserverCallback);
    current_context = plugin->ContextCreate(0);
}

API::~API() {
    plugin->ContextDestroy(0);
    delete plugin;
}

void API::ContextCreate()
{
    DebugLog("Context Created!");
    current_context = plugin->ContextCreate(0);
}

void API::ContextDestroy()
{
    plugin->ContextDestroy(0);
    DebugLog("Context Destroyed!");
}

PeerConnectionObject* API::CreatePeerConnection()
{
    PeerConnectionObject* pco = plugin->ContextCreatePeerConnection(current_context);
    current_pco = pco;
    return pco;
}

void API::DeletePeerConnection(PeerConnectionObject* pco) { plugin->ContextDeletePeerConnection(current_context, pco); }

void API::SetLocalDescription(API::RTCSdpType type, char* sdp)
{
    switch (type)
    {
    case RTCSdpType::Offer:
        RTCSessionDescription desc = { webrtc::RTCSdpType::Offer, sdp };
        char** error;
        RTCErrorType errorType;

        if (errorType != RTCErrorType::NONE)
        {
            DebugLog("%s", error);
        }
        break;
    }
}

void API::SetRemoteDescription(API::RTCSdpType type, char* sdp) { }

void API::AddICECandidate(char* candidate, char* sdpMLineIndex, int sdpMid) { }

void API::CreateOffer()
{
    RTCOfferAnswerOptions options = { true, true };
    plugin->PeerConnectionCreateOffer(current_context, current_pco, &options);
}

void API::CreateAnswer()
{
    RTCOfferAnswerOptions options = { true, true };
    plugin->PeerConnectionCreateAnswer(current_context, current_pco, &options);
}

API::SignalingState API::GetSignallingState(PeerConnectionObject* pco)
{
    PeerConnectionInterface::SignalingState state = plugin->PeerConnectionSignalingState(pco);
    switch (state)
    {
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
