#pragma once

#include <iostream>

#include "pch.h"

namespace webrtc
{

#if defined(__linux__)
#define SPACE_INTERFACE_EXPORT __attribute__((visibility("default")))
#endif

namespace webrtc = ::webrtc;

class Context;

class PeerConnectionObject;

class CSDO;

class PeerConnectionInterface;

enum class RTCSdpType
{
    Offer,
    PrAnswer,
    Answer,
    Rollback
};

struct RTCSessionDescription
{
    RTCSdpType type;
    const char* sdp;
};

enum class RTCPeerConnectionState
{
    New,
    Connecting,
    Connected,
    Disconnected,
    Failed,
    Closed
};

enum class RTCIceConnectionState
{
    New,
    Checking,
    Connected,
    Completed,
    Failed,
    Disconnected,
    Closed,
    Max
};

enum class RTCSignalingState
{
    Stable,
    HaveLocalOffer,
    HaveRemoteOffer,
    HaveLocalPranswer,
    HaveRemotePranswer,
    Closed
};

enum class RTCPeerConnectionEventType
{
    ConnectionStateChange,
    DataChannel,
    IceCandidate,
    IceConnectionStateChange,
    Track
};

enum class SdpSemanticsType
{
    UnifiedPlan
};

enum class RTCIceCredentialType
{
    Password,
    OAuth
};

enum class TrackKind
{
    Audio,
    Video
};

struct RTCOfferAnswerOptions
{
    bool iceRestart;
    bool voiceActivityDetection;
};

struct RTCIceServer
{
    char* credential;
    char* credentialType;
    char** urls;
    int urlsLength;
    char* username;
};

struct RTCConfiguration
{
    RTCIceServer* iceServers;
    int iceServersLength;
    char* iceServerPolicy;
};

struct RTCIceCandidate
{
    char* candidate;
    char* sdpMid;
    int sdpMLineIndex;
};

struct RTCIceCandidateInit
{
    char* candidate;
    char* sdpMid;
    int32_t sdpMLineIndex;
};

char* ConvertString(const std::string str);

struct Candidate
{
    char* candidate;
    int32_t component;
    char* foundation;
    char* ip;
    uint16_t port;
    uint32_t priority;
    char* address;
    char* protocol;
    char* relatedAddress;
    uint16_t relatedPort;
    char* tcpType;
    char* type;
    char* usernameFragment;

    Candidate& operator=(const cricket::Candidate& obj)
    {
        candidate = ConvertString(obj.ToString());
        component = obj.component();
        foundation = ConvertString(obj.foundation());
        ip = ConvertString(obj.address().ipaddr().ToString());
        port = obj.address().port();
        priority = obj.priority();
        address = ConvertString(obj.address().ToString());
        protocol = ConvertString(obj.protocol());
        relatedAddress = ConvertString(obj.related_address().ToString());
        relatedPort = obj.related_address().port();
        tcpType = ConvertString(obj.tcptype());
        type = ConvertString(obj.type());
        usernameFragment = ConvertString(obj.username());
        return *this;
    }
};

// Callback Delegates
using DelegateCreateSDSuccess = void (*)(PeerConnectionObject*, RTCSdpType, const char*);

using DelegateCreateSDFailure = void (*)(PeerConnectionObject*, webrtc::RTCErrorType, const char*);
using DelegateLocalSdpReady = void (*)(PeerConnectionObject*, const char*, const char*);
using DelegateIceCandidate = void (*)(PeerConnectionObject*, const char*, const char*, const int);
using DelegateOnIceConnectionChange =
    void (*)(PeerConnectionObject*, webrtc::PeerConnectionInterface::IceConnectionState);
using DelegateOnIceGatheringChange =
    void (*)(PeerConnectionObject*, webrtc::PeerConnectionInterface::IceGatheringState);
using DelegateOnConnectionStateChange =
    void (*)(PeerConnectionObject*, webrtc::PeerConnectionInterface::PeerConnectionState);
using DelegateOnDataChannel = void (*)(PeerConnectionObject*, webrtc::DataChannelInterface*);
using DelegateOnRenegotiationNeeded = void (*)(PeerConnectionObject*);
using DelegateOnTrack = void (*)(PeerConnectionObject*, webrtc::RtpTransceiverInterface*);
using DelegateOnRemoveTrack = void (*)(PeerConnectionObject*, webrtc::RtpReceiverInterface*);

////////////////////////////////////
class WebRTCPlugin
{
private:
    const std::string stun_server_url = "stun:stun.l.google.com:19302";

public:
    PeerConnectionObject* ContextCreatePeerConnection(Context* context);

    void PeerConnectionRegisterCallbackCreateSD(
        PeerConnectionObject* obj, DelegateCreateSDSuccess onSuccess, DelegateCreateSDFailure onFailure);

    void PeerConnectionRegisterOnIceCandidate(PeerConnectionObject* obj, DelegateIceCandidate callback);

    CSDO* PeerConnectionCreateOffer(Context* context, PeerConnectionObject* obj, const RTCOfferAnswerOptions* options);

    CSDO* PeerConnectionCreateAnswer(Context* context, PeerConnectionObject* obj, const RTCOfferAnswerOptions* options);

    webrtc::RTCErrorType PeerConnectionSetLocalDescription(
        Context* context, PeerConnectionObject* obj, const RTCSessionDescription* desc, std::string& error);

    webrtc::RTCErrorType PeerConnectionSetRemoteDescription(
        Context* context, PeerConnectionObject* obj, const RTCSessionDescription* desc, std::string& error);

    webrtc::PeerConnectionInterface::SignalingState PeerConnectionSignalingState(PeerConnectionObject* obj);

    webrtc::RTCErrorType
    PeerConnectionAddIceCandidate(PeerConnectionObject* obj, char* candidate, char* sdpMLineIndex, int sdpMid);

    std::string GetEnvVarOrDefault(const char* env_var_name, const char* default_value)
    {
        std::string value;
        const char* env_var = getenv(env_var_name);
        if (env_var)
            value = env_var;

        if (value.empty())
            value = default_value;

        return value;
    }

    std::string GetPeerConnectionString()
    {
        return stun_server_url;
        return GetEnvVarOrDefault("WEBRTC_CONNECT", "stun:stun.l.google.com:19302");
    }
};
}
