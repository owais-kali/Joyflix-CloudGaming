#pragma once

#include "MediaStreamObserverX.h"
#include "PeerConnectionObject.h"

namespace webrtc
{
const std::map<std::string, uint32_t> statsTypes = { { "codec", 0 },
                                                     { "inbound-rtp", 1 },
                                                     { "outbound-rtp", 2 },
                                                     { "remote-inbound-rtp", 3 },
                                                     { "remote-outbound-rtp", 4 },
                                                     { "media-source", 5 },
                                                     { "csrc", 6 },
                                                     { "peer-connection", 7 },
                                                     { "data-channel", 8 },
                                                     { "stream", 9 },
                                                     { "track", 10 },
                                                     { "transceiver", 11 },
                                                     { "sender", 12 },
                                                     { "receiver", 13 },
                                                     { "transport", 14 },
                                                     { "sctp-transport", 15 },
                                                     { "candidate-pair", 16 },
                                                     { "local-candidate", 17 },
                                                     { "remote-candidate", 18 },
                                                     { "certificate", 19 },
                                                     { "ice-server", 20 } };

enum class RTCSdpType;
class SSDO;

struct ContextDependencies
{
};

class ContextManager
{
public:
    static ContextManager* GetInstance();
    ~ContextManager();

    Context* GetContext(int uid) const;
    Context* CreateContext(int uid, ContextDependencies& dependencies);
    void DestroyContext(int uid);
    void SetCurContext(Context*);
    bool Exists(Context* context);
    using ContextPtr = std::unique_ptr<Context>;
    Context* curContext = nullptr;
    std::mutex mutex;

private:
    std::map<int, ContextPtr> m_contexts;
    static std::unique_ptr<ContextManager> s_instance;
};

class Context
{
public:
    Context(ContextDependencies& dependencies);
    ~Context();

    bool ExistsRefPtr(const rtc::RefCountInterface* ptr) const { return m_mapRefPtr.find(ptr) != m_mapRefPtr.end(); }

    template<typename T>
    void AddRefPtr(rtc::scoped_refptr<T> refptr)
    {
        m_mapRefPtr.emplace(refptr.get(), refptr);
    }

    void AddRefPtr(rtc::RefCountInterface* ptr) { m_mapRefPtr.emplace(ptr, ptr); }

    template<typename T>
    void RemoveRefPtr(rtc::scoped_refptr<T>& refptr)
    {
        std::lock_guard<std::mutex> lock(mutex);
        m_mapRefPtr.erase(refptr.get());
    }

    template<typename T>
    void RemoveRefPtr(T* ptr)
    {
        std::lock_guard<std::mutex> lock(mutex);
        m_mapRefPtr.erase(ptr);
    }

    // MediaStream
    rtc::scoped_refptr<MediaStreamInterface> CreateMediaStream(const std::string& streamId);
    void RegisterMediaStreamObserver(webrtc::MediaStreamInterface* stream);
    void UnRegisterMediaStreamObserver(webrtc::MediaStreamInterface* stream);
    MediaStreamObserverX* GetObserver(const webrtc::MediaStreamInterface* stream);

    // Audio Source
    rtc::scoped_refptr<AudioSourceInterface> CreateAudioSource();
    // Audio Renderer
    AudioTrackSinkAdapter* CreateAudioTrackSinkAdapter();
    void DeleteAudioTrackSinkAdapter(AudioTrackSinkAdapter* sink);

    // Video Source
    rtc::scoped_refptr<VideoTrackSource> CreateVideoSource();

    // MediaStreamTrack
    rtc::scoped_refptr<VideoTrackInterface>
    CreateVideoTrack(const std::string& label, webrtc::VideoTrackSourceInterface* source);
    rtc::scoped_refptr<AudioTrackInterface>
    CreateAudioTrack(const std::string& label, webrtc::AudioSourceInterface* source);
    void StopMediaStreamTrack(webrtc::MediaStreamTrackInterface* track);

    // PeerConnection
    PeerConnectionObject* CreatePeerConnection(const webrtc::PeerConnectionInterface::RTCConfiguration& config);
    void DeletePeerConnection(PeerConnectionObject* obj);

    // StatsReport
    std::mutex mutexStatsReport;
    void AddStatsReport(const rtc::scoped_refptr<const webrtc::RTCStatsReport>& report);
    const RTCStats** GetStatsList(const RTCStatsReport* report, size_t* length, uint32_t** types);
    void DeleteStatsReport(const webrtc::RTCStatsReport* report);

    // DataChannel
    DataChannelInterface*
    CreateDataChannel(PeerConnectionObject* obj, const char* label, const DataChannelInit& options);
    void AddDataChannel(rtc::scoped_refptr<DataChannelInterface> channel, PeerConnectionObject& pc);
    DataChannelObject* GetDataChannelObject(const DataChannelInterface* channel);
    void DeleteDataChannel(DataChannelInterface* channel);

    // mutex;
    std::mutex mutex;

private:
    std::unique_ptr<rtc::Thread> m_workerThread;
    std::unique_ptr<rtc::Thread> m_signalingThread;
    std::unique_ptr<TaskQueueFactory> m_taskQueueFactory;
    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> m_peerConnectionFactory;
    std::vector<rtc::scoped_refptr<const webrtc::RTCStatsReport>> m_listStatsReport;
    std::map<const PeerConnectionObject*, std::unique_ptr<PeerConnectionObject>> m_mapClients;
    std::map<const webrtc::MediaStreamInterface*, std::unique_ptr<MediaStreamObserverX>> m_mapMediaStreamObserver;
    std::map<const webrtc::PeerConnectionInterface*, rtc::scoped_refptr<SetSessionDescriptionObserver>>
        m_mapSetSessionDescriptionObserver;
    std::map<const DataChannelInterface*, std::unique_ptr<DataChannelObject>> m_mapDataChannels;
    std::map<const rtc::RefCountInterface*, rtc::scoped_refptr<rtc::RefCountInterface>> m_mapRefPtr;

    static uint32_t s_rendererId;

    static uint32_t GenerateRendererId();
};

} // namespace webrtc
