#include "_API.h"
#include "Context.h"
#include "Logger.h"
#include "WebRTCPlugin.h"
#include "map"
#include "future"

namespace webrtc
{
static API* api;
static WebRTCPlugin* plugin;
static Context* current_context;
static std::map<PeerConnectionObject*, RTCPeerConnection*> PCOs;

void OnSessionDescriptionObserverCallback(
    PeerConnectionObject* pco,
    CreateSessionDescriptionObserver* csdo,
    RTCSdpType rtcSdpType,
    const char* desc,
    RTCErrorType errorType,
    const char* errorMsg)
{
    auto callback = PCOs[pco]->LocalDescriptionCallback;
    if(callback != nullptr){
        callback(rtcSdpType, desc, errorType, errorMsg);
    }
    else{
        DebugError("SDP Created but OnLocalDescription callback is not set");
    }
}

RTCPeerConnection::RTCPeerConnection()
{
    pco = plugin->ContextCreatePeerConnection(current_context);
    PCOs[pco] = this;
}
RTCPeerConnection::RTCPeerConnection(const char* config) { }

RTCPeerConnection::~RTCPeerConnection()
{
    plugin->ContextDeletePeerConnection(current_context, pco);
    PCOs.erase(pco);
}

void RTCPeerConnection::CreateOffer(const RTCOfferAnswerOptions& options) {
    webrtc::RTCOfferAnswerOptions Options = {
        options.iceRestart,
        options.voiceActivityDetection
    };
    plugin->PeerConnectionCreateOffer(current_context, pco, &Options);
}
void RTCPeerConnection::CreateAnswer(const RTCOfferAnswerOptions& options) {
    webrtc::RTCOfferAnswerOptions Options = {
        options.iceRestart,
        options.voiceActivityDetection
    };
    plugin->PeerConnectionCreateAnswer(current_context, pco, &options);
}

void RTCPeerConnection::OnLocalDescription(DelegateOnLocalDescription callback) {
    LocalDescriptionCallback = callback;
}

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

API::~API()
{
    plugin->ContextDestroy(0);
    delete plugin;
}

void API::ContextCreate(uint id)
{
    DebugLog("Context Created!");
    current_context = plugin->ContextCreate(0);
}

void API::SetCurrentContext(uint ID) { current_context = ContextManager::GetInstance()->GetContext(ID); }

void API::ContextDestroy(uint ID)
{
    plugin->ContextDestroy(0);
    DebugLog("Context Destroyed!");
}

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
}