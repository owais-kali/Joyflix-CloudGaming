#include "SetLocalDescriptionObserver.h"

namespace webrtc
{
DelegateSetLocalDesc SetLocalDescriptionObserver::s_setLocalDescCallback = nullptr;

rtc::scoped_refptr<SetLocalDescriptionObserver>
webrtc::SetLocalDescriptionObserver::Create(webrtc::PeerConnectionObject* connection)
{
    return rtc::make_ref_counted<SetLocalDescriptionObserver>(connection);
}

webrtc::SetLocalDescriptionObserver::SetLocalDescriptionObserver(webrtc::PeerConnectionObject* connection)
{
    m_connection = connection;
}

void webrtc::SetLocalDescriptionObserver::OnSetLocalDescriptionComplete(webrtc::RTCError error)
{
    s_setLocalDescCallback(m_connection, this, error.type(), error.message());
}

}