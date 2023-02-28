#include "pch.h"

#include "PeerConnectionObject.h"
#include "PeerConnectionStatsCollectorCallback.h"

namespace webrtc
{

rtc::scoped_refptr<PeerConnectionStatsCollectorCallback>
webrtc::PeerConnectionStatsCollectorCallback::Create(webrtc::PeerConnectionObject* connection)
{
    return rtc::make_ref_counted<PeerConnectionStatsCollectorCallback>(connection);
}
void webrtc::PeerConnectionStatsCollectorCallback::OnStatsDelivered(
    const rtc::scoped_refptr<const RTCStatsReport>& report)
{
    m_owner->ReceiveStatsReport(report);
    s_collectStatsCallback(m_owner, this, report.get());
}

}