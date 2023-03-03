
#pragma once
#include "Types.h"

typedef unsigned int uint;

namespace webrtc
{
class API;
class WebRTCPlugin;
class Context;
class PeerConnectionObject;
class CreateSessionDescriptionObserver;
class SetRemoteDescriptionObserver;

enum class RTCSdpType;
enum class RTCErrorType;

using DelegateOnLocalDescription = void (*)(RTCSdpType, const char*, RTCErrorType, const char*);
using DelegateOnSetRemoteDescription = void (*)(RTCErrorType, const char*);

class RTCPeerConnection
{
private:
    PeerConnectionObject* pco;
    DelegateOnLocalDescription LocalDescriptionCallback;
    DelegateOnSetRemoteDescription SetRemoteDescriptionCallback;
public:
    enum class EventType
    {

    };

    RTCPeerConnection();
    RTCPeerConnection(const char* config);
    ~RTCPeerConnection();

    void CreateOffer(const RTCOfferAnswerOptions& options);
    void CreateAnswer(const RTCOfferAnswerOptions& options);
    void OnLocalDescription(DelegateOnLocalDescription callback);

    void SetLocalDescription(const RTCSessionDescription sdp);
    void SetRemoteDescription(const RTCSessionDescription sdp);

    friend void OnSessionDescriptionObserverCallback(
        PeerConnectionObject* pco,
        CreateSessionDescriptionObserver* observer,
        RTCSdpType rtcSdpType,
        const char* desc,
        RTCErrorType errorType,
        const char* errorMsg);

    friend void OnSetRemoteDescriptionObserverCallback(
        PeerConnectionObject* pco, SetRemoteDescriptionObserver* observer, RTCErrorType errorType, const char* errorMsg);
};

void OnSessionDescriptionObserverCallback(
    PeerConnectionObject* pco,
    CreateSessionDescriptionObserver* observer,
    RTCSdpType rtcSdpType,
    const char* desc,
    RTCErrorType errorType,
    const char* errorMsg);

void OnSetRemoteDescriptionObserverCallback(
    PeerConnectionObject* pco, SetRemoteDescriptionObserver* observer, RTCErrorType errorType, const char* errorMsg);

class API
{
public:
    enum SignalingState
    {
        kStable,
        kHaveLocalOffer,
        kHaveLocalPrAnswer,
        kHaveRemoteOffer,
        kHaveRemotePrAnswer,
        kClosed,
    };

    API();
    ~API();

    void ContextCreate(uint ID);
    void SetCurrentContext(uint ID);
    void ContextDestroy(uint ID);

    void AddICECandidate(char* candidate, char* sdpMLineIndex, int sdpMid);

    SignalingState GetSignallingState(PeerConnectionObject* pco);
};
}