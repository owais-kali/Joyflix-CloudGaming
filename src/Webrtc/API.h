#pragma once
typedef unsigned int uint;

namespace webrtc
{
class API;
class WebRTCPlugin;
class Context;
class PeerConnectionObject;

class RTCPeerConnection{
private:
    PeerConnectionObject* pco;
public:
    RTCPeerConnection();
    RTCPeerConnection(const char* config);
};

class API
{
private:
    Context* current_context;
    PeerConnectionObject* current_pco;

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

    void ContextCreate();
    void ContextDestroy();

    PeerConnectionObject* CreatePeerConnection();
    void DeletePeerConnection(PeerConnectionObject* pco);

    void SetLocalDescription(RTCSdpType type, char* sdp);

    void SetRemoteDescription(RTCSdpType type, char* sdp);

    void AddICECandidate(char* candidate, char* sdpMLineIndex, int sdpMid);

    void CreateOffer();
    void CreateAnswer();

    SignalingState GetSignallingState(PeerConnectionObject* pco);
};
}