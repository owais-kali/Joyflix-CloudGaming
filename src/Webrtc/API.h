#pragma once

#define _Context void*

namespace webrtc {
    class API {

    public:
        enum class RTCSdpType { Offer, PrAnswer, Answer, Rollback };

        API();
        ~API();
        _Context ContextCreate();
        void ContextDestroy();
        void StartWebRTCServer();

        void SetLocalDescription(RTCSdpType type, char* sdp);
        void SetRemoteDescription(RTCSdpType type, char* sdp);

        void CreateAnswer();
    };
}