#pragma once
#define _Context void*

namespace webrtc {
    class API {
    public:
        enum class RTCSdpType { Offer, PrAnswer, Answer, Rollback };
        using DelegateOnGotDescription = void (*)(webrtc::API::RTCSdpType, char*);
        using DelegateOnGotICECandidate = void (*)(char*, char*, int);

        DelegateOnGotDescription GotDescriptionCallback;
        DelegateOnGotICECandidate GotICECandidateCallback;

        API(DelegateOnGotDescription onGotDescriptionCallback, DelegateOnGotICECandidate onGotICECandidateCallback);
        ~API();
        _Context ContextCreate();
        void ContextDestroy();
        void StartWebRTCServer();

        void SetLocalDescription(RTCSdpType type, char* sdp);
        void SetRemoteDescription(RTCSdpType type, char* sdp);

        void CreateAnswer();


    };
}