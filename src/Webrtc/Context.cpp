#pragma clang diagnostic ignored "-Wunreachable-code"
#pragma clang diagnostic ignored "-Wunused-variable"

#include "Context.h"
#include <api/create_peerconnection_factory.h>
#include <api/task_queue/default_task_queue_factory.h>
#include <rtc_base/ssl_adapter.h>
#include <rtc_base/strings/json.h>

#include <iostream>

#include "api/audio_codecs/builtin_audio_decoder_factory.h"
#include "api/audio_codecs/builtin_audio_encoder_factory.h"

#include "Logger.h"

namespace webrtc
{

using namespace webrtc;

std::unique_ptr<ContextManager> ContextManager::s_instance;
ContextManager* ContextManager::GetInstance()
{
    if (s_instance == nullptr)
    {
        s_instance = std::make_unique<ContextManager>();
    }
    return s_instance.get();
}

Context* ContextManager::GetContext(int uid) const
{
    auto it = s_instance->m_contexts.find(uid);
    if (it != s_instance->m_contexts.end())
    {
        return it->second.get();
    }
    return nullptr;
}

Context* ContextManager::CreateContext(int uid, ContextDependencies& dependencies)
{
    auto it = s_instance->m_contexts.find(uid);
    if (it != s_instance->m_contexts.end())
    {
        DebugLog("Using already created context with ID %d", uid);
        return nullptr;
    }
    s_instance->m_contexts[uid] = std::make_unique<Context>(dependencies);
    return s_instance->m_contexts[uid].get();
}

void ContextManager::SetCurContext(Context* context) { curContext = context; }

bool ContextManager::Exists(Context* context)
{
    for (auto it = s_instance->m_contexts.begin(); it != s_instance->m_contexts.end(); ++it)
    {
        if (it->second.get() == context)
            return true;
    }
    return false;
}

void ContextManager::DestroyContext(int uid)
{
    auto it = s_instance->m_contexts.find(uid);
    if (it != s_instance->m_contexts.end())
    {
        s_instance->m_contexts.erase(it);
    }
}

ContextManager::~ContextManager()
{
    if (m_contexts.size())
    {
        DebugWarning("%lu remaining context(s) registered", m_contexts.size());
    }
    m_contexts.clear();
}

bool Convert(const std::string& str, webrtc::PeerConnectionInterface::RTCConfiguration& config)
{
    config = PeerConnectionInterface::RTCConfiguration {};
    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    Json::Value configJson;
    Json::String err;
    auto ok = reader->parse(str.c_str(), str.c_str() + static_cast<int>(str.length()), &configJson, &err);
    if (!ok)
    {
        // json parse failed.
        return false;
    }

    Json::Value iceServersJson = configJson["iceServers"];
    if (!iceServersJson)
        return false;
    for (auto iceServerJson : iceServersJson)
    {
        webrtc::PeerConnectionInterface::IceServer iceServer;
        for (auto url : iceServerJson["urls"])
        {
            iceServer.urls.push_back(url.asString());
        }
        if (!iceServerJson["username"].isNull())
        {
            iceServer.username = iceServerJson["username"].asString();
        }
        if (!iceServerJson["credential"].isNull())
        {
            iceServer.password = iceServerJson["credential"].asString();
        }
        config.servers.push_back(iceServer);
    }
    Json::Value iceTransportPolicy = configJson["iceTransportPolicy"];
    if (iceTransportPolicy["hasValue"].asBool())
    {
        config.type = static_cast<PeerConnectionInterface::IceTransportsType>(iceTransportPolicy["value"].asInt());
    }
    Json::Value iceCandidatePoolSize = configJson["iceCandidatePoolSize"];
    if (iceCandidatePoolSize["hasValue"].asBool())
    {
        config.ice_candidate_pool_size = iceCandidatePoolSize["value"].asInt();
    }
    Json::Value bundlePolicy = configJson["bundlePolicy"];
    if (bundlePolicy["hasValue"].asBool())
    {
        config.bundle_policy = static_cast<PeerConnectionInterface::BundlePolicy>(bundlePolicy["value"].asInt());
    }
    config.sdp_semantics = webrtc::SdpSemantics::kUnifiedPlan;
    config.enable_implicit_rollback = true;
    return true;
}

Context::Context(ContextDependencies& dependencies)
    : m_workerThread(rtc::Thread::CreateWithSocketServer())
    , m_signalingThread(rtc::Thread::CreateWithSocketServer())
    , m_taskQueueFactory(CreateDefaultTaskQueueFactory())
{
    m_workerThread->Start();
    m_signalingThread->Start();

    rtc::InitializeSSL();

    m_peerConnectionFactory = webrtc::CreatePeerConnectionFactory(
        m_workerThread.get() /* network_thread */,
        m_workerThread.get() /* worker_thread */,
        m_signalingThread.get(),
        nullptr /* default_adm */,
        CreateBuiltinAudioEncoderFactory(),
        CreateBuiltinAudioDecoderFactory(),
        CreateBuiltinVideoEncoderFactory(),
        CreateBuiltinVideoDecoderFactory(),
        nullptr /* audio_mixer */,
        nullptr /* audio_processing */);

    if (!m_peerConnectionFactory)
    {
        std::cout << "Failed to initialize PeerConnectionFactory" << std::endl;
    }
}

Context::~Context()
{
    {
        std::lock_guard<std::mutex> lock(mutex);

        m_peerConnectionFactory = nullptr;
        // m_workerThread->BlockingCall([this]() { m_audioDevice = nullptr; });
        m_mapClients.clear();

        // check count of refptr to avoid to forget disposing
        RTC_DCHECK_EQ(m_mapRefPtr.size(), 0);

        m_mapRefPtr.clear();
        m_mapMediaStreamObserver.clear();
        m_mapDataChannels.clear();
        //          m_mapVideoRenderer.clear();

        m_workerThread->Quit();
        m_workerThread.reset();
        m_signalingThread->Quit();
        m_signalingThread.reset();
    }
}

rtc::scoped_refptr<MediaStreamInterface> Context::CreateMediaStream(const std::string& streamId)
{
    return m_peerConnectionFactory->CreateLocalMediaStream(streamId);
}

void Context::RegisterMediaStreamObserver(webrtc::MediaStreamInterface* stream)
{

}

void Context::UnRegisterMediaStreamObserver(webrtc::MediaStreamInterface* stream)
{
    m_mapMediaStreamObserver.erase(stream);
}

MediaStreamObserverX* Context::GetObserver(const webrtc::MediaStreamInterface* stream) { return m_mapMediaStreamObserver[stream].get(); }

PeerConnectionObject* Context::CreatePeerConnection(const webrtc::PeerConnectionInterface::RTCConfiguration& config)
{
    std::unique_ptr<PeerConnectionObject> obj = std::make_unique<PeerConnectionObject>(*this);
    PeerConnectionDependencies dependencies(obj.get());
    auto result = m_peerConnectionFactory->CreatePeerConnectionOrError(config, std::move(dependencies));
    if (!result.ok())
    {
        RTC_LOG(LS_ERROR) << result.error().message();
        return nullptr;
    }
    obj->connection = result.MoveValue();
    PeerConnectionObject* ptr = obj.get();
    m_mapClients[ptr] = std::move(obj);
    return ptr;
}

void Context::DeletePeerConnection(PeerConnectionObject* obj) { m_mapClients.erase(obj); }

void Context::AddStatsReport(const rtc::scoped_refptr<const webrtc::RTCStatsReport>& report)
{
    std::lock_guard<std::mutex> lock(mutexStatsReport);
    m_listStatsReport.push_back(report);
}

const RTCStats** Context::GetStatsList(const RTCStatsReport* report, size_t* length, uint32_t** types)
{
    NOT_IMPLEMENTED("GetStatsList");
}

void Context::DeleteStatsReport(const webrtc::RTCStatsReport* report)
{
    std::lock_guard<std::mutex> lock(mutexStatsReport);

    auto result = std::find_if(
        m_listStatsReport.begin(),
        m_listStatsReport.end(),
        [report](rtc::scoped_refptr<const webrtc::RTCStatsReport> it) { return it.get() == report; });

    if (result == m_listStatsReport.end())
    {
        RTC_LOG(LS_INFO) << "Calling DeleteStatsReport is failed. The reference of RTCStatsReport is not found.";
        return;
    }
    m_listStatsReport.erase(result);
}

uint32_t Context::s_rendererId = 0;
uint32_t Context::GenerateRendererId() { return s_rendererId++; }

DataChannelInterface*
Context::CreateDataChannel(PeerConnectionObject* obj, const char* label, const DataChannelInit& options)
{
    const RTCErrorOr<rtc::scoped_refptr<DataChannelInterface>> result =
        obj->connection->CreateDataChannelOrError(label, &options);

    if (!result.ok())
        return nullptr;

    rtc::scoped_refptr<DataChannelInterface> channel = result.value();

    AddDataChannel(channel, *obj);
    return channel.get();
}

void Context::AddDataChannel(rtc::scoped_refptr<DataChannelInterface> channel, PeerConnectionObject& pc)
{
    auto dataChannelObj = std::make_unique<DataChannelObject>(channel, pc);
    m_mapDataChannels[channel.get()] = std::move(dataChannelObj);
}

DataChannelObject* Context::GetDataChannelObject(const DataChannelInterface* channel)
{
    return m_mapDataChannels[channel].get();
}

void Context::DeleteDataChannel(DataChannelInterface* channel)
{
    if (m_mapDataChannels.count(channel) > 0)
    {
        m_mapDataChannels.erase(channel);
    }
}
rtc::scoped_refptr<AudioSourceInterface> Context::CreateAudioSource()
{
    return rtc::scoped_refptr<AudioSourceInterface>();
}
AudioTrackSinkAdapter* Context::CreateAudioTrackSinkAdapter() { return nullptr; }
void Context::DeleteAudioTrackSinkAdapter(AudioTrackSinkAdapter* sink) { }
rtc::scoped_refptr<VideoTrackSource> Context::CreateVideoSource() { return rtc::scoped_refptr<VideoTrackSource>(); }
rtc::scoped_refptr<VideoTrackInterface>
Context::CreateVideoTrack(const std::string& label, webrtc::VideoTrackSourceInterface* source)
{
    return rtc::scoped_refptr<VideoTrackInterface>();
}
rtc::scoped_refptr<AudioTrackInterface>
Context::CreateAudioTrack(const std::string& label, webrtc::AudioSourceInterface* source)
{
    return rtc::scoped_refptr<AudioTrackInterface>();
}
void Context::StopMediaStreamTrack(webrtc::MediaStreamTrackInterface* track) { }

} // namespace webrtc