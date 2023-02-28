#pragma once

#include "PeerConnectionObject.h"
#include "MSO.h"

namespace webrtc{

enum class RTCSdpType;

class SSDO;

class Context
{
public:
    Context();

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

    MSO* GetObserver(const webrtc::MediaStreamInterface* stream);

    // PeerConnection
    PeerConnectionObject* CreatePeerConnection(const webrtc::PeerConnectionInterface::RTCConfiguration& config);

    void DeletePeerConnection(PeerConnectionObject* obj);

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
    std::map<const webrtc::MediaStreamInterface*, std::unique_ptr<MSO>> m_mapMediaStreamObserver;
    std::map<const webrtc::PeerConnectionInterface*, rtc::scoped_refptr<SetSessionDescriptionObserver>>
        m_mapSetSessionDescriptionObserver;
    std::map<const DataChannelInterface*, std::unique_ptr<DataChannelObject>> m_mapDataChannels;
    std::map<const rtc::RefCountInterface*, rtc::scoped_refptr<rtc::RefCountInterface>> m_mapRefPtr;

    static uint32_t s_rendererId;

    static uint32_t GenerateRendererId();
};

} // namespace webrtc
