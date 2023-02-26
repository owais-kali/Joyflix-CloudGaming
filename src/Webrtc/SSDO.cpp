#include "SSDO.h"

#include "PeerConnectionObject.h"
#include "pch.h"

namespace joyflix {
    namespace webrtc {
        rtc::scoped_refptr<SSDO> SSDO::Create(PeerConnectionObject *connection) {
            return rtc::scoped_refptr<SSDO>(new rtc::RefCountedObject<SSDO>(connection));
        }

        SSDO::SSDO(PeerConnectionObject *connection) {
            m_connection = connection;
        }

        void SSDO::OnSuccess() {}

        void SSDO::OnFailure(webrtc::RTCError error) {}
    }
}