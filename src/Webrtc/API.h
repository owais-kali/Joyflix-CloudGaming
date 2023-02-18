#pragma once

namespace webrtc {
    class API {
    public:
        API();

        ~API();

        void* ContextCreate();

        void ContextDestroy(void* ctx);

    };
}