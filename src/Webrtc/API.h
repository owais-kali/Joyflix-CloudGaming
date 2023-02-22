#pragma once
#define _Context void*

namespace webrtc {
    class API {
    public:
        enum class RTCSdpType { Offer, PrAnswer, Answer, Rollback };
        using DelegateOnGotDescription = void (*)(webrtc::API::RTCSdpType, char*);
    private:
        DelegateOnGotDescription GotDescriptionCallback;
    public:
        API(DelegateOnGotDescription onGotDescriptionCallback);
        ~API();
        _Context ContextCreate();
        void ContextDestroy();
        void StartWebRTCServer();

        void SetLocalDescription(RTCSdpType type, char* sdp);
        void SetRemoteDescription(RTCSdpType type, char* sdp);

        void CreateAnswer();


    };
}