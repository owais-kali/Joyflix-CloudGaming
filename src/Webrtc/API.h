#pragma once

#define _Context void*

namespace webrtc {
    class API {
    public:
        API();
        ~API();
        _Context ContextCreate();
        void ContextDestroy();

        void StartWebRTCServer();
    };
}