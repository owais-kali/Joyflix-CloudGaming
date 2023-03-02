#include "pch.h"

#include "CreateSessionDescriptionObserverX.h"
#include "PeerConnectionObject.h"

namespace webrtc
{
DelegateCreateSessionDesc CreateSessionDescriptionObserverX::s_createSessionDescCallback = nullptr;

rtc::scoped_refptr<CreateSessionDescriptionObserverX>
CreateSessionDescriptionObserverX::Create(PeerConnectionObject* connection)
{
    return rtc::make_ref_counted<CreateSessionDescriptionObserverX>(connection);
}

CreateSessionDescriptionObserverX::CreateSessionDescriptionObserverX(PeerConnectionObject* connection) { m_connection = connection; }

void CreateSessionDescriptionObserverX::OnSuccess(SessionDescriptionInterface* desc)
{
    std::string out;
    desc->ToString(&out);
    const auto sdpType = ConvertSdpType(desc->GetType());
    s_createSessionDescCallback(m_connection, this, sdpType, out.c_str(), RTCErrorType::NONE, nullptr);
}

void CreateSessionDescriptionObserverX::OnFailure(webrtc::RTCError error)
{
    s_createSessionDescCallback(m_connection, this, RTCSdpType(), nullptr, error.type(), error.message());
}

} // end namespace webrtc
