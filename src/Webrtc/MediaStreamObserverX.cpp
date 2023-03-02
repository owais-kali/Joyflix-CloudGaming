#include "pch.h"

#include "Context.h"
#include "MediaStreamObserverX.h"

namespace webrtc
{
MediaStreamObserverX::MediaStreamObserverX(MediaStreamInterface* stream)
    : ::webrtc::MediaStreamObserver(
          stream,
          [this](AudioTrackInterface* track, MediaStreamInterface* stream) {
              this->OnAudioTrackAdded(track, stream);
          },
          [this](AudioTrackInterface* track, MediaStreamInterface* stream) {
              this->OnAudioTrackRemoved(track, stream);
          },
          [this](VideoTrackInterface* track, MediaStreamInterface* stream) {
              this->OnVideoTrackAdded(track, stream);
          },
          [this](VideoTrackInterface* track, MediaStreamInterface* stream) {
              this->OnVideoTrackRemoved(track, stream);
          })
{ }
void MediaStreamObserverX::RegisterOnAddTrack(DelegateMediaStreamOnAddTrack callback) { }
void MediaStreamObserverX::RegisterOnRemoveTrack(DelegateMediaStreamOnRemoveTrack callback) { }
void MediaStreamObserverX::OnVideoTrackAdded(
    VideoTrackInterface* track, MediaStreamInterface* stream)
{
}
void MediaStreamObserverX::OnAudioTrackAdded(
    AudioTrackInterface* track, MediaStreamInterface* stream)
{
}
void MediaStreamObserverX::OnVideoTrackRemoved(
    VideoTrackInterface* track, MediaStreamInterface* stream)
{
}
void MediaStreamObserverX::OnAudioTrackRemoved(
    AudioTrackInterface* track, MediaStreamInterface* stream)
{
}
}