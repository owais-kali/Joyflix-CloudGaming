#pragma once
#include "PeerConnectionObject.h"

namespace webrtc {

enum class RTCSdpType;

class Context {
 public:
  Context();

  // PeerConnection
  PeerConnectionObject* CreatePeerConnection(
      const webrtc::PeerConnectionInterface::RTCConfiguration& config);
  void AddObserver(
      const webrtc::PeerConnectionInterface* connection,
      const rtc::scoped_refptr<SetSessionDescriptionObserver>& observer);
  SetSessionDescriptionObserver* GetObserver(
      webrtc::PeerConnectionInterface* connection);

  void AddTracks();

 private:
  std::unique_ptr<rtc::Thread> m_workerThread;
  std::unique_ptr<rtc::Thread> m_signalingThread;
  std::unique_ptr<TaskQueueFactory> m_taskQueueFactory;
  rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface>
      m_peerConnectionFactory;
  // rtc::scoped_refptr<DummyAudioDevice> m_audioDevice;
  std::vector<rtc::scoped_refptr<const webrtc::RTCStatsReport>>
      m_listStatsReport;
  std::map<const PeerConnectionObject*,
           rtc::scoped_refptr<PeerConnectionObject>>
      m_mapClients;
  std::map<const webrtc::MediaStreamInterface*,
           std::unique_ptr<MediaStreamObserver>>
      m_mapMediaStreamObserver;
  std::map<const webrtc::PeerConnectionInterface*,
           rtc::scoped_refptr<SetSessionDescriptionObserver>>
      m_mapSetSessionDescriptionObserver;
  // std::map<const DataChannelInterface*, std::unique_ptr<DataChannelObject>>
  // m_mapDataChannels; std::map<const uint32_t,
  // std::shared_ptr<UnityVideoRenderer>> m_mapVideoRenderer; std::map<const
  // AudioTrackSinkAdapter*, std::unique_ptr<AudioTrackSinkAdapter>>
  // m_mapAudioTrackAndSink;
  std::map<const rtc::RefCountInterface*,
           rtc::scoped_refptr<rtc::RefCountInterface>>
      m_mapRefPtr;

  static uint32_t s_rendererId;
  static uint32_t GenerateRendererId();
};

webrtc::SdpType ConvertSdpType(RTCSdpType type);
RTCSdpType ConvertSdpType(webrtc::SdpType type);

}  // namespace webrtc
