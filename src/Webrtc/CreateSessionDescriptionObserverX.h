#pragma once

#include <api/jsep.h>

#include "WebRTCPlugin.h"

namespace webrtc
{
class CreateSessionDescriptionObserverX : public ::webrtc::CreateSessionDescriptionObserver
{
public:
    static rtc::scoped_refptr<CreateSessionDescriptionObserverX> Create(PeerConnectionObject* connection);
    static void RegisterCallback(DelegateCreateSessionDesc callback) { s_createSessionDescCallback = callback; }

    void OnSuccess(SessionDescriptionInterface* desc) override;
    void OnFailure(webrtc::RTCError error) override;

protected:
    explicit CreateSessionDescriptionObserverX(PeerConnectionObject* connection);
    ~CreateSessionDescriptionObserverX() override = default;

private:
    PeerConnectionObject* m_connection;
    static DelegateCreateSessionDesc s_createSessionDescCallback;
};
}
