#include "SetRemoteDescriptionObserver.h"

namespace webrtc
{
DelegateSetRemoteDesc SetRemoteDescriptionObserver::s_setRemoteDescCallback = nullptr;

rtc::scoped_refptr<SetRemoteDescriptionObserver>
webrtc::SetRemoteDescriptionObserver::Create(webrtc::PeerConnectionObject* connection)
{
    return rtc::make_ref_counted<SetRemoteDescriptionObserver>(connection);
}
webrtc::SetRemoteDescriptionObserver::SetRemoteDescriptionObserver(webrtc::PeerConnectionObject* connection)
{
    m_connection = connection;
}

void webrtc::SetRemoteDescriptionObserver::OnSetRemoteDescriptionComplete(webrtc::RTCError error)
{
    s_setRemoteDescCallback(m_connection, this, error.type(), error.message());
}
}