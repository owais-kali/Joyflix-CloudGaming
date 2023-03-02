#pragma once

#include <list>
#include <pc/media_stream_observer.h>

#include "WebRTCPlugin.h"

namespace webrtc
{
    class Context;
    class MediaStreamObserverX : public MediaStreamObserver
    {
    public:
        explicit MediaStreamObserverX(webrtc::MediaStreamInterface* stream);
        void RegisterOnAddTrack(DelegateMediaStreamOnAddTrack callback);
        void RegisterOnRemoveTrack(DelegateMediaStreamOnRemoveTrack callback);

    private:
        void OnVideoTrackAdded(webrtc::VideoTrackInterface* track, webrtc::MediaStreamInterface* stream);
        void OnAudioTrackAdded(webrtc::AudioTrackInterface* track, webrtc::MediaStreamInterface* stream);
        void OnVideoTrackRemoved(webrtc::VideoTrackInterface* track, webrtc::MediaStreamInterface* stream);
        void OnAudioTrackRemoved(webrtc::AudioTrackInterface* track, webrtc::MediaStreamInterface* stream);

        std::list<DelegateMediaStreamOnAddTrack> m_listOnAddTrack;
        std::list<DelegateMediaStreamOnRemoveTrack> m_listOnRemoveTrack;
    };

} // end namespace webrtc