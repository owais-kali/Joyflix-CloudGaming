#include "pch.h"

#include <rtc_base/strings/json.h>

#include "Context.h"
#include "PeerConnectionObject.h"

namespace webrtc
{
webrtc::SdpType ConvertSdpType(RTCSdpType type)
{
    switch (type)
    {
    case RTCSdpType::Offer:
        return webrtc::SdpType::kOffer;
    case RTCSdpType::PrAnswer:
        return webrtc::SdpType::kPrAnswer;
    case RTCSdpType::Answer:
        return webrtc::SdpType::kAnswer;
    case RTCSdpType::Rollback:
        return webrtc::SdpType::kRollback;
    }
    throw std::invalid_argument("Unknown RTCSdpType");
}

RTCSdpType ConvertSdpType(webrtc::SdpType type)
{
    switch (type)
    {
    case webrtc::SdpType::kOffer:
        return RTCSdpType::Offer;
    case webrtc::SdpType::kPrAnswer:
        return RTCSdpType::PrAnswer;
    case webrtc::SdpType::kAnswer:
        return RTCSdpType::Answer;
    case webrtc::SdpType::kRollback:
        return RTCSdpType::Rollback;
    }
    throw std::invalid_argument("Unknown SdpType");
}

PeerConnectionObject::PeerConnectionObject(Context& context)
    : context(context)
{
}
PeerConnectionObject::~PeerConnectionObject() { }
void PeerConnectionObject::Close() { }
RTCErrorType PeerConnectionObject::SetLocalDescription(
    const RTCSessionDescription& desc,
    rtc::scoped_refptr<SetLocalDescriptionObserverInterface> observer,
    std::string& error)
{
    return RTCErrorType::NONE;
}
RTCErrorType PeerConnectionObject::SetLocalDescriptionWithoutDescription(
    rtc::scoped_refptr<SetLocalDescriptionObserverInterface> observer, std::string& error)
{
    return RTCErrorType::NONE;
}
RTCErrorType PeerConnectionObject::SetRemoteDescription(
    const RTCSessionDescription& desc, rtc::scoped_refptr<SetRemoteDescriptionObserverInterface>, std::string& error)
{
    return RTCErrorType::NONE;
}
bool PeerConnectionObject::GetSessionDescription(
    const SessionDescriptionInterface* sdp, RTCSessionDescription& desc) const
{
    return true;
}
RTCErrorType PeerConnectionObject::SetConfiguration(const std::string& config) { return RTCErrorType::NONE; }
std::string PeerConnectionObject::GetConfiguration() const { return nullptr; }
void PeerConnectionObject::CreateOffer(const RTCOfferAnswerOptions& options, CreateSessionDescriptionObserver* observer)
{
    webrtc::PeerConnectionInterface::RTCOfferAnswerOptions _options;
    _options.ice_restart = options.iceRestart;
    _options.voice_activity_detection = options.voiceActivityDetection;
    connection->CreateOffer(observer, _options);
}
void PeerConnectionObject::CreateAnswer(
    const RTCOfferAnswerOptions& options, CreateSessionDescriptionObserver* observer)
{
}
void PeerConnectionObject::ReceiveStatsReport(const rtc::scoped_refptr<const RTCStatsReport>& report) { }
void PeerConnectionObject::OnSignalingChange(PeerConnectionInterface::SignalingState new_state) { }
void PeerConnectionObject::OnAddStream(rtc::scoped_refptr<MediaStreamInterface> stream)
{
    PeerConnectionObserver::OnAddStream(stream);
}
void PeerConnectionObject::OnRemoveStream(rtc::scoped_refptr<MediaStreamInterface> stream)
{
    PeerConnectionObserver::OnRemoveStream(stream);
}
void PeerConnectionObject::OnDataChannel(rtc::scoped_refptr<DataChannelInterface> data_channel) { }
void PeerConnectionObject::OnRenegotiationNeeded() { PeerConnectionObserver::OnRenegotiationNeeded(); }
void PeerConnectionObject::OnIceConnectionChange(PeerConnectionInterface::IceConnectionState new_state)
{
    PeerConnectionObserver::OnIceConnectionChange(new_state);
}
void PeerConnectionObject::OnConnectionChange(PeerConnectionInterface::PeerConnectionState new_state)
{
    PeerConnectionObserver::OnConnectionChange(new_state);
}
void PeerConnectionObject::OnIceGatheringChange(PeerConnectionInterface::IceGatheringState new_state) { }
void PeerConnectionObject::OnIceCandidate(const IceCandidateInterface* candidate) { }
void PeerConnectionObject::OnTrack(rtc::scoped_refptr<RtpTransceiverInterface> transceiver)
{
    PeerConnectionObserver::OnTrack(transceiver);
}
void PeerConnectionObject::OnRemoveTrack(rtc::scoped_refptr<RtpReceiverInterface> receiver)
{
    PeerConnectionObserver::OnRemoveTrack(receiver);
}
}