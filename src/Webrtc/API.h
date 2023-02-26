#pragma once
#define _Context void*

namespace webrtc {
    class API {
    public:
        enum class RTCSdpType {
            Offer, PrAnswer, Answer, Rollback
        };
        enum SignalingState {
            kStable,
            kHaveLocalOffer,
            kHaveLocalPrAnswer,
            kHaveRemoteOffer,
            kHaveRemotePrAnswer,
            kClosed,
        };

        using DelegateOnGotDescription = void (*)(RTCSdpType, char *);
        using DelegateOnGotICECandidate = void (*)(char *, char *, int);

        DelegateOnGotDescription GotDescriptionCallback;
        DelegateOnGotICECandidate GotICECandidateCallback;

        API(DelegateOnGotDescription onGotDescriptionCallback, DelegateOnGotICECandidate onGotICECandidateCallback);

        ~API();

        _Context ContextCreate();

        void ContextDestroy();

        void StartWebRTCServer();

        void SetLocalDescription(RTCSdpType type, char *sdp);

        void SetRemoteDescription(RTCSdpType type, char *sdp);

        void AddICECandidate(char *candidate, char *sdpMLineIndex, int sdpMid);

        void CreateAnswer();

        SignalingState GetSignallingState();
    };
}