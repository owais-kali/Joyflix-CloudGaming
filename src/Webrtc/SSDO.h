#pragma once

#include "WebRTCPlugin.h"

namespace joyflix {
    namespace webrtc {
        class SSDO : public webrtc::SetSessionDescriptionObserver {
        public:
            static rtc::scoped_refptr<SSDO> Create(PeerConnectionObject *connection);

            void OnSuccess() override;

            void OnFailure(webrtc::RTCError error) override;

        protected:
            explicit SSDO(PeerConnectionObject *connection);

            ~SSDO() = default;

        private:
            PeerConnectionObject *m_connection;
        };
    }
}