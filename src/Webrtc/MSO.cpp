#include "pch.h"

#include "Context.h"
#include "MSO.h"

namespace webrtc
{
webrtc::MSO::MSO(webrtc::MediaStreamInterface* stream)
    : ::webrtc::MediaStreamObserver(
          stream,
          [this](webrtc::AudioTrackInterface* track, webrtc::MediaStreamInterface* stream) {
              this->OnAudioTrackAdded(track, stream);
          },
          [this](webrtc::AudioTrackInterface* track, webrtc::MediaStreamInterface* stream) {
              this->OnAudioTrackRemoved(track, stream);
          },
          [this](webrtc::VideoTrackInterface* track, webrtc::MediaStreamInterface* stream) {
              this->OnVideoTrackAdded(track, stream);
          },
          [this](webrtc::VideoTrackInterface* track, webrtc::MediaStreamInterface* stream) {
              this->OnVideoTrackRemoved(track, stream);
          })
{ }
void webrtc::MSO::RegisterOnAddTrack(DelegateMediaStreamOnAddTrack callback) { }
void webrtc::MSO::RegisterOnRemoveTrack(DelegateMediaStreamOnRemoveTrack callback) { }
void webrtc::MSO::OnVideoTrackAdded(
    webrtc::VideoTrackInterface* track, webrtc::MediaStreamInterface* stream)
{
}
void webrtc::MSO::OnAudioTrackAdded(
    webrtc::AudioTrackInterface* track, webrtc::MediaStreamInterface* stream)
{
}
void webrtc::MSO::OnVideoTrackRemoved(
    webrtc::VideoTrackInterface* track, webrtc::MediaStreamInterface* stream)
{
}
void webrtc::MSO::OnAudioTrackRemoved(
    webrtc::AudioTrackInterface* track, webrtc::MediaStreamInterface* stream)
{
}
}