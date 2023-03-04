#pragma once

typedef unsigned int uint;

namespace webrtc
{
class API;
class WebRTCPlugin;
class Context;
class PeerConnectionObject;
class CreateSessionDescriptionObserver;
class SetLocalDescriptionObserver;
class SetRemoteDescriptionObserver;

struct RTCOfferAnswerOptions;
struct RTCSessionDescription;

enum class RTCSdpType;
enum class RTCErrorType;

using DelegateOnLocalDescription = void (*)(RTCSdpType, const char*);

using DelegateOnSetLocalDescription = void (*)(RTCErrorType, const char*);
using DelegateOnSetRemoteDescription = void (*)(RTCErrorType, const char*);
using DelegateOnIceCandidate = void (*)(PeerConnectionObject* ,const char* candidate, const char* sdpMid, const int sdpMLineIndex);

class RTCPeerConnection
{
private:
    PeerConnectionObject* pco;
    DelegateOnLocalDescription LocalDescriptionCallback = nullptr;

    DelegateOnSetLocalDescription SetLocalDescriptionCallback = nullptr;
    DelegateOnSetRemoteDescription SetRemoteDescriptionCallback = nullptr;

public:
    enum class EventType
    {

    };

    explicit RTCPeerConnection();
    ~RTCPeerConnection();

    void CreateOffer(const RTCOfferAnswerOptions& options);
    void CreateAnswer(const RTCOfferAnswerOptions& options);
    void OnLocalDescription(DelegateOnLocalDescription callback) { LocalDescriptionCallback = callback; };

    void SetLocalDescription(const RTCSessionDescription sdp);
    void SetRemoteDescription(const RTCSessionDescription sdp);

    void OnIceCandidate(DelegateOnIceCandidate callback);
    void AddIceCandidate(char* candidate, char* sdpMLineIndex, int sdpMid);

    friend void OnSessionDescriptionObserverCallback(
        PeerConnectionObject* pco,
        CreateSessionDescriptionObserver* observer,
        RTCSdpType rtcSdpType,
        const char* desc,
        RTCErrorType errorType,
        const char* errorMsg);

    friend void OnSetLocalDescriptionObserverCallback(
        PeerConnectionObject* pco, SetLocalDescriptionObserver* observer, RTCErrorType errorType, const char* errorMsg);

    friend void OnSetRemoteDescriptionObserverCallback(
        PeerConnectionObject* pco,
        SetRemoteDescriptionObserver* observer,
        RTCErrorType errorType,
        const char* errorMsg);
};

void OnSessionDescriptionObserverCallback(
    PeerConnectionObject* pco,
    CreateSessionDescriptionObserver* observer,
    RTCSdpType rtcSdpType,
    const char* desc,
    RTCErrorType errorType,
    const char* errorMsg);

void OnSetLocalDescriptionObserverCallback(
    PeerConnectionObject* pco, SetLocalDescriptionObserver* observer, RTCErrorType errorType, const char* errorMsg);

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