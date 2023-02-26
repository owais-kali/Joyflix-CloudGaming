
#pragma once
#include "WebRTCPlugin.h"

namespace webrtc {
using namespace webrtc;

class PeerConnectionObject;
enum class RTCSdpType;

class PeerConnectionObject : public webrtc::PeerConnectionObserver {
 public:
  PeerConnectionObject();

  // webrtc::PeerConnectionObserver
  // Triggered when the SignalingState changed.
  void OnSignalingChange(
      webrtc::PeerConnectionInterface::SignalingState new_state) override;
  // Triggered when media is received on a new stream from remote peer.
  void OnAddStream(
      rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override;
  // Triggered when a remote peer closes a stream.
  void OnRemoveStream(
      rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override;
  // Triggered when a remote peer opens a data channel.
  void OnDataChannel(
      rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) override;
  // Triggered when renegotiation is needed. For example, an ICE restart
  // has begun.
  void OnRenegotiationNeeded() override;
  // Called any time the IceConnectionState changes.
  void OnIceConnectionChange(
      PeerConnectionInterface::IceConnectionState new_state) override;
  // Called any time the PeerConnectionState changes.
  virtual void OnConnectionChange(
      PeerConnectionInterface::PeerConnectionState new_state) override;
  // Called any time the IceGatheringState changes.
  void OnIceGatheringChange(
      PeerConnectionInterface::IceGatheringState new_state) override;
  // A new ICE candidate has been gathered.
  void OnIceCandidate(const IceCandidateInterface* candidate) override;
  // Ice candidates have been removed.
  void OnIceCandidatesRemoved(
      const std::vector<cricket::Candidate>& candidates) override {}
  // Called when the ICE connection receiving status changes.
  void OnIceConnectionReceivingChange(bool Receiving) override {}
  // This is called when signaling indicates a transceiver will be receiving
  // media from the remote endpoint. This is fired during a call to
  // SetRemoteDescription. The receiving track can be accessed by:
  // |transceiver->receiver()->track()| and its associated streams by
  // |transceiver->receiver()->streams()|.
  // Note: This will only be called if Unified Plan semantics are specified.
  // This behavior is specified in section 2.2.8.2.5 of the "Set the
  // RTCSessionDescription" algorithm:
  // https://w3c.github.io/webrtc-pc/#set-description
  void OnTrack(
      rtc::scoped_refptr<webrtc::RtpTransceiverInterface> transceiver) override;

  void OnRemoveTrack(
      rtc::scoped_refptr<RtpReceiverInterface> receiver) override;

  friend class DataChannelObject;

  DelegateCreateSDSuccess onCreateSDSuccess = nullptr;
  DelegateCreateSDFailure onCreateSDFailure = nullptr;
  DelegateIceCandidate onIceCandidate = nullptr;
  DelegateLocalSdpReady onLocalSdpReady = nullptr;
  DelegateOnConnectionStateChange onConnectionStateChange = nullptr;
  DelegateOnIceConnectionChange onIceConnectionChange = nullptr;
  DelegateOnIceGatheringChange onIceGatheringChange = nullptr;
  DelegateOnDataChannel onDataChannel = nullptr;
  DelegateOnRenegotiationNeeded onRenegotiationNeeded = nullptr;
  DelegateOnTrack onTrack = nullptr;
  DelegateOnRemoveTrack onRemoveTrack = nullptr;
  rtc::scoped_refptr<webrtc::PeerConnectionInterface> connection = nullptr;

 public:
    void CreateOffer(const RTCOfferAnswerOptions& options, CreateSessionDescriptionObserver* observer);
    void CreateAnswer(const RTCOfferAnswerOptions& options, CreateSessionDescriptionObserver* observer);

    void RegisterCallbackCreateSD(DelegateCreateSDSuccess onSuccess,
                                DelegateCreateSDFailure onFailure) {
    onCreateSDSuccess = onSuccess;
    onCreateSDFailure = onFailure;
  }

  void RegisterLocalSdpReady(DelegateLocalSdpReady callback) {
    onLocalSdpReady = callback;
  }
  void RegisterIceCandidate(DelegateIceCandidate callback) {
    onIceCandidate = callback;
  }

  RTCErrorType SetLocalDescription(
      const RTCSessionDescription& desc,
      webrtc::SetSessionDescriptionObserver* observer,
      std::string& error);

  RTCErrorType SetRemoteDescription(
      const RTCSessionDescription& desc,
      webrtc::SetSessionDescriptionObserver* observer,
      std::string& error);

  void AddTracks(
      webrtc::PeerConnectionFactoryInterface* peer_connection_factory_) const;
};

// conductor.cc constants
const char kAudioLabel[] = "audio_label";
const char kVideoLabel[] = "video_label";
const char kStreamId[] = "stream_id";
const uint16_t kDefaultServerPort = 8888;
}  // namespace webrtc
