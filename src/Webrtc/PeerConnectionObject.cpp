#include "PeerConnectionObject.h"
#include "Context.h"
#include "Logger.h"
#include <iostream>

namespace webrtc
{
PeerConnectionObject::PeerConnectionObject(Context& context)
    : context(context)
{
}

PeerConnectionObject::~PeerConnectionObject()
{
    if (connection == nullptr)
    {
        return;
    }
    auto senders = connection->GetSenders();
    for (const auto& sender : senders)
    {
        // ignore error.
        connection->RemoveTrackOrError(sender);
    }

    const auto state = connection->peer_connection_state();
    if (state != webrtc::PeerConnectionInterface::PeerConnectionState::kClosed)
    {
        connection->Close();
    }
    connection = nullptr;
}

void PeerConnectionObject::OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> channel)
{
    NOT_IMPLEMENTED("OnDataChannel");
}

void PeerConnectionObject::OnIceCandidate(const webrtc::IceCandidateInterface* candidate)
{
    NOT_IMPLEMENTED("OnIceCandidate");
}

void PeerConnectionObject::OnRenegotiationNeeded()
{
    NOT_IMPLEMENTED("OnRenegotiationNeeded");
}

void PeerConnectionObject::OnTrack(rtc::scoped_refptr<webrtc::RtpTransceiverInterface> transceiver)
{
    NOT_IMPLEMENTED("OnTrack");
}

void PeerConnectionObject::OnRemoveTrack(rtc::scoped_refptr<RtpReceiverInterface> receiver)
{
    NOT_IMPLEMENTED("OnRemoveTrack");
}

// Called any time the IceConnectionState changes.
void PeerConnectionObject::OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state)
{
    DebugLog("OnIceConnectionChange %d", new_state);
    if (onIceConnectionChange != nullptr)
    {
        onIceConnectionChange(this, new_state);
    }
}

void PeerConnectionObject::OnConnectionChange(PeerConnectionInterface::PeerConnectionState new_state)
{
    //        DebugLog("OnConnectionChange %d", new_state);
    //        if(onConnectionStateChange != nullptr)
    //        {
    //            onConnectionStateChange(this, new_state);
    //        }
}

// Called any time the IceGatheringState changes.
void PeerConnectionObject::OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state)
{
    //        DebugLog("OnIceGatheringChange %d", new_state);
    //        if (onIceGatheringChange != nullptr)
    //        {
    //            onIceGatheringChange(this, new_state);
    //        }
}

void PeerConnectionObject::OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state)
{
    //        DebugLog("OnSignalingChange %d", new_state);
}

void PeerConnectionObject::OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream)
{
    //        DebugLog("OnAddStream");
}

void PeerConnectionObject::OnRemoveStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream)
{
    //        DebugLog("OnRemoveStream");
}

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
    webrtc::PeerConnectionInterface::RTCOfferAnswerOptions _options;
    _options.ice_restart = options.iceRestart;
    _options.voice_activity_detection = options.voiceActivityDetection;
    connection->CreateAnswer(observer, _options);
}

RTCErrorType PeerConnectionObject::SetLocalDescription(
    const RTCSessionDescription& desc, webrtc::SetSessionDescriptionObserver* observer, std::string& error)
{
    SdpParseError error_;
    std::unique_ptr<SessionDescriptionInterface> _desc =
        CreateSessionDescription(ConvertSdpType(desc.type), desc.sdp, &error_);
    if (!_desc.get())
    {
        error = error_.description;
        return RTCErrorType::SYNTAX_ERROR;
    }
    connection->SetLocalDescription(observer, _desc.release());
    return RTCErrorType::NONE;
}

RTCErrorType PeerConnectionObject::SetRemoteDescription(
    const RTCSessionDescription& desc, webrtc::SetSessionDescriptionObserver* observer, std::string& error)
{
    SdpParseError error_;
    std::unique_ptr<SessionDescriptionInterface> _desc =
        CreateSessionDescription(ConvertSdpType(desc.type), desc.sdp, &error_);
    if (!_desc.get())
    {
        error = error_.description;
        return RTCErrorType::SYNTAX_ERROR;
    }
    connection->SetRemoteDescription(observer, _desc.release());
    return RTCErrorType::NONE;
}

void PeerConnectionObject::AddTracks(webrtc::PeerConnectionFactoryInterface* peer_connection_factory_) const
{
    if (!connection->GetSenders().empty())
    {
        return; // Already added tracks.
    }
    rtc::scoped_refptr<webrtc::AudioTrackInterface> audio_track(peer_connection_factory_->CreateAudioTrack(
        kAudioLabel, peer_connection_factory_->CreateAudioSource(cricket::AudioOptions()).get()));
    auto result_or_error = connection->AddTrack(audio_track, { kStreamId });
    if (!result_or_error.ok())
    {
        RTC_LOG(LS_ERROR) << "Failed to add audio track to PeerConnection: " << result_or_error.error().message();
    }
}

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

} // namespace webrtc