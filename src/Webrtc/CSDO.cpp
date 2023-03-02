#include "pch.h"

#include "CSDO.h"
#include "PeerConnectionObject.h"

namespace webrtc
{
DelegateCreateSessionDesc CSDO::s_createSessionDescCallback = nullptr;

rtc::scoped_refptr<CSDO> CSDO::Create(PeerConnectionObject* connection)
{
    return rtc::make_ref_counted<CSDO>(connection);
}

CSDO::CSDO(PeerConnectionObject* connection) { m_connection = connection; }

void CSDO::OnSuccess(SessionDescriptionInterface* desc)
{
    std::string out;
    desc->ToString(&out);
    const auto sdpType = ConvertSdpType(desc->GetType());
    s_createSessionDescCallback(m_connection, this, sdpType, out.c_str(), RTCErrorType::NONE, nullptr);
}

void CSDO::OnFailure(webrtc::RTCError error)
{
    s_createSessionDescCallback(m_connection, this, RTCSdpType(), nullptr, error.type(), error.message());
}

} // end namespace webrtc
