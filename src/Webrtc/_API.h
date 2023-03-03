
#pragma once
typedef unsigned int uint;

namespace webrtc
{
class API;
class WebRTCPlugin;
class Context;
class PeerConnectionObject;
class CreateSessionDescriptionObserver;
enum class RTCSdpType;
enum class RTCErrorType;

struct RTCOfferAnswerOptions;

using DelegateOnLocalDescription = void (*)(RTCSdpType, const char*, RTCErrorType, const char*);

class RTCPeerConnection
{
private:
    PeerConnectionObject* pco;
    DelegateOnLocalDescription LocalDescriptionCallback;
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

    friend void OnSessionDescriptionObserverCallback(
        PeerConnectionObject* pco,
        CreateSessionDescriptionObserver* csdo,
        RTCSdpType rtcSdpType,
        const char* desc,
        RTCErrorType errorType,
        const char* errorMsg);
};

void OnSessionDescriptionObserverCallback(
    PeerConnectionObject* pco,
    CreateSessionDescriptionObserver* csdo,
    RTCSdpType rtcSdpType,
    const char* desc,
    RTCErrorType errorType,
    const char* errorMsg);

class API
{
public:
    enum class RTCSdpType
    {
        Offer,
        PrAnswer,
        Answer,
        Rollback
    };
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

    void SetLocalDescription(RTCSdpType type, char* sdp);

    void SetRemoteDescription(RTCSdpType type, char* sdp);

    void AddICECandidate(char* candidate, char* sdpMLineIndex, int sdpMid);

    SignalingState GetSignallingState(PeerConnectionObject* pco);
};
}