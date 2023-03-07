//
// Created by owais on 25/8/22.
//
#pragma clang diagnostic ignored "-Wunused-variable"

#include "WebRTCPlugin.h"
#include "Context.h"
#include "CreateSessionDescriptionObserverX.h"
#include "Logger.h"
#include "SetLocalDescriptionObserver.h"
#include "SetRemoteDescriptionObserver.h"
#include "api/jsep.h"

namespace webrtc
{

MediaStreamInterface* WebRTCPlugin::ContextCreateMediaStream(Context* context, const char* streamId)
{
    rtc::scoped_refptr<MediaStreamInterface> stream = context->CreateMediaStream(streamId);
    context->AddRefPtr(stream);
    return stream.get();
}

void WebRTCPlugin::ContextRegisterMediaStreamObserver(Context* context, MediaStreamInterface* stream)
{
    context->RegisterMediaStreamObserver(stream);
}

void WebRTCPlugin::ContextUnRegisterMediaStreamObserver(Context* context, MediaStreamInterface* stream)
{
    context->UnRegisterMediaStreamObserver(stream);
}

MediaStreamTrackInterface*
WebRTCPlugin::ContextCreateVideoTrack(Context* context, const char* label, webrtc::VideoTrackSourceInterface* source)
{
    rtc::scoped_refptr<VideoTrackInterface> track = context->CreateVideoTrack(label, source);
    context->AddRefPtr(track);
    return track.get();
}

void WebRTCPlugin::ContextStopMediaStreamTrack(Context* context, ::webrtc::MediaStreamTrackInterface* track)
{
    context->StopMediaStreamTrack(track);
}

webrtc::VideoTrackSourceInterface* WebRTCPlugin::ContextCreateVideoTrackSource(Context* context)
{
    rtc::scoped_refptr<VideoTrackSourceInterface> source = context->CreateVideoSource();
    context->AddRefPtr(source);
    return source.get();
}

webrtc::AudioSourceInterface* WebRTCPlugin::ContextCreateAudioTrackSource(Context* context)
{
    rtc::scoped_refptr<AudioSourceInterface> source = context->CreateAudioSource();
    context->AddRefPtr(source);
    return source.get();
}

webrtc::MediaStreamTrackInterface*
WebRTCPlugin::ContextCreateAudioTrack(Context* context, const char* label, webrtc::AudioSourceInterface* source)
{
    rtc::scoped_refptr<AudioTrackInterface> track = context->CreateAudioTrack(label, source);
    context->AddRefPtr(track);
    return track.get();
}

void WebRTCPlugin::ContextAddRefPtr(Context* context, rtc::RefCountInterface* ptr) { context->AddRefPtr(ptr); }

void WebRTCPlugin::ContextDeleteRefPtr(Context* context, rtc::RefCountInterface* ptr) { context->RemoveRefPtr(ptr); }

bool WebRTCPlugin::MediaStreamAddTrack(MediaStreamInterface* stream, MediaStreamTrackInterface* track)
{
    if (track->kind() == "audio")
    {
        return stream->AddTrack(rtc::scoped_refptr<AudioTrackInterface>(static_cast<AudioTrackInterface*>(track)));
    }
    else
    {
        return stream->AddTrack(rtc::scoped_refptr<VideoTrackInterface>(static_cast<VideoTrackInterface*>(track)));
    }
}

bool WebRTCPlugin::MediaStreamRemoveTrack(MediaStreamInterface* stream, MediaStreamTrackInterface* track)
{
    if (track->kind() == "audio")
    {
        return stream->RemoveTrack(rtc::scoped_refptr<AudioTrackInterface>(static_cast<AudioTrackInterface*>(track)));
    }
    else
    {
        return stream->RemoveTrack(rtc::scoped_refptr<VideoTrackInterface>(static_cast<VideoTrackInterface*>(track)));
    }
}

char* WebRTCPlugin::MediaStreamGetID(MediaStreamInterface* stream) { NOT_IMPLEMENTED("MediaStreamGetID"); }

void WebRTCPlugin::MediaStreamRegisterOnAddTrack(
    Context* context, MediaStreamInterface* stream, DelegateMediaStreamOnAddTrack callback)
{
}

void WebRTCPlugin::MediaStreamRegisterOnRemoveTrack(
    Context* context, MediaStreamInterface* stream, DelegateMediaStreamOnRemoveTrack callback)
{
}

VideoTrackInterface** WebRTCPlugin::MediaStreamGetVideoTracks(MediaStreamInterface* stream, size_t* length)
{
    return nullptr;
}

AudioTrackInterface** WebRTCPlugin::MediaStreamGetAudioTracks(MediaStreamInterface* stream, size_t* length)
{
    return nullptr;
}

VideoTrackSourceInterface* WebRTCPlugin::ContextGetVideoSource(Context* context, VideoTrackInterface* track)
{
    return nullptr;
}

TrackKind WebRTCPlugin::MediaStreamTrackGetKind(MediaStreamTrackInterface* track) { return TrackKind::Audio; }

MediaStreamTrackInterface::TrackState WebRTCPlugin::MediaStreamTrackGetReadyState(MediaStreamTrackInterface* track)
{
    return MediaStreamTrackInterface::kEnded;
}

char* WebRTCPlugin::MediaStreamTrackGetID(MediaStreamTrackInterface* track) { return nullptr; }

bool WebRTCPlugin::MediaStreamTrackGetEnabled(MediaStreamTrackInterface* track) { return false; }

void WebRTCPlugin::MediaStreamTrackSetEnabled(MediaStreamTrackInterface* track, bool enabled) { }

UnityVideoRenderer*
WebRTCPlugin::CreateVideoRenderer(Context* context, DelegateVideoFrameResize callback, bool needFlipVertical)
{
    return nullptr;
}

uint32_t WebRTCPlugin::GetVideoRendererId(UnityVideoRenderer* sink) { return 0; }

void WebRTCPlugin::DeleteVideoRenderer(Context* context, UnityVideoRenderer* sink) { }

void WebRTCPlugin::VideoTrackAddOrUpdateSink(VideoTrackInterface* track, UnityVideoRenderer* sink) { }

void WebRTCPlugin::VideoTrackRemoveSink(VideoTrackInterface* track, UnityVideoRenderer* sink) { }

Context* WebRTCPlugin::ContextCreate(int uid)
{
    auto ctx = ContextManager::GetInstance()->GetContext(uid);
    if (ctx != nullptr)
    {
        DebugLog("Already created context with ID %d", uid);
        return ctx;
    }
    ContextDependencies dependencies = {};
    ctx = ContextManager::GetInstance()->CreateContext(uid, dependencies);
    return ctx;
}

void WebRTCPlugin::ContextDestroy(int uid) { ContextManager::GetInstance()->DestroyContext(uid); }

PeerConnectionObject* WebRTCPlugin::ContextCreatePeerConnection(Context* context)
{
    PeerConnectionInterface::RTCConfiguration config;
    config.sdp_semantics = SdpSemantics::kUnifiedPlan;
    config.enable_implicit_rollback = true;
    return context->CreatePeerConnection(config);
}

PeerConnectionObject* WebRTCPlugin::ContextCreatePeerConnectionWithConfig(Context* context, const char* conf)
{
    PeerConnectionInterface::RTCConfiguration config;
    if (!Convert(conf, config))
        return nullptr;

    config.sdp_semantics = SdpSemantics::kUnifiedPlan;
    config.enable_implicit_rollback = true;
    return context->CreatePeerConnection(config);
}

void WebRTCPlugin::ContextDeletePeerConnection(Context* context, PeerConnectionObject* obj)
{
    obj->Close();
    context->DeletePeerConnection(obj);
}

void WebRTCPlugin::PeerConnectionClose(PeerConnectionObject* obj) { obj->Close(); }

void WebRTCPlugin::PeerConnectionRestartIce(PeerConnectionObject* obj) { }

RTCErrorType WebRTCPlugin::PeerConnectionAddTrack(
    PeerConnectionObject* obj, MediaStreamTrackInterface* track, const char* streamId, RtpSenderInterface** sender)
{
    return RTCErrorType::INVALID_PARAMETER;
}

RtpTransceiverInterface*
WebRTCPlugin::PeerConnectionAddTransceiver(PeerConnectionObject* obj, MediaStreamTrackInterface* track)
{
    return nullptr;
}

RtpTransceiverInterface* WebRTCPlugin::PeerConnectionAddTransceiverWithInit(
    PeerConnectionObject* obj, MediaStreamTrackInterface* track, const RTCRtpTransceiverInit* init)
{
    return nullptr;
}

RtpTransceiverInterface*
WebRTCPlugin::PeerConnectionAddTransceiverWithType(PeerConnectionObject* obj, cricket::MediaType type)
{
    return nullptr;
}

RtpTransceiverInterface* WebRTCPlugin::PeerConnectionAddTransceiverWithTypeAndInit(
    PeerConnectionObject* obj, cricket::MediaType type, const RTCRtpTransceiverInit* init)
{
    return nullptr;
}

RTCErrorType WebRTCPlugin::PeerConnectionRemoveTrack(PeerConnectionObject* obj, RtpSenderInterface* sender)
{
    return RTCErrorType::INVALID_PARAMETER;
}

RTCErrorType WebRTCPlugin::PeerConnectionSetConfiguration(PeerConnectionObject* obj, const char* conf)
{
    return RTCErrorType::INVALID_PARAMETER;
}

char* WebRTCPlugin::PeerConnectionGetConfiguration(PeerConnectionObject* obj) { return nullptr; }

PeerConnectionStatsCollectorCallback* WebRTCPlugin::PeerConnectionGetStats(PeerConnectionObject* obj)
{
    return nullptr;
}

PeerConnectionStatsCollectorCallback*
WebRTCPlugin::PeerConnectionSenderGetStats(PeerConnectionObject* obj, RtpSenderInterface* sender)
{
    return nullptr;
}

PeerConnectionStatsCollectorCallback*
WebRTCPlugin::PeerConnectionReceiverGetStats(PeerConnectionObject* obj, RtpReceiverInterface* receiver)
{
    return nullptr;
}

const RTCStats**
WebRTCPlugin::ContextGetStatsList(Context* context, const RTCStatsReport* report, size_t* length, uint32_t** types)
{
    return nullptr;
}

void WebRTCPlugin::ContextDeleteStatsReport(Context* context, const RTCStatsReport* report) { }

const char* WebRTCPlugin::StatsGetJson(const RTCStats* stats) { return nullptr; }

int64_t WebRTCPlugin::StatsGetTimestamp(const RTCStats* stats) { return 0; }

const char* WebRTCPlugin::StatsGetId(const RTCStats* stats) { return nullptr; }

uint32_t WebRTCPlugin::StatsGetType(const RTCStats* stats) { return 0; }

const RTCStatsMemberInterface** WebRTCPlugin::StatsGetMembers(const RTCStats* stats, size_t* length) { return nullptr; }

bool WebRTCPlugin::StatsMemberIsDefined(const RTCStatsMemberInterface* member) { return false; }

const char* WebRTCPlugin::StatsMemberGetName(const RTCStatsMemberInterface* member) { return nullptr; }

bool WebRTCPlugin::StatsMemberGetBool(const RTCStatsMemberInterface* member) { return false; }

int32_t WebRTCPlugin::StatsMemberGetInt(const RTCStatsMemberInterface* member) { return 0; }

uint32_t WebRTCPlugin::StatsMemberGetUnsignedInt(const RTCStatsMemberInterface* member) { return 0; }

int64_t WebRTCPlugin::StatsMemberGetLong(const RTCStatsMemberInterface* member) { return 0; }

uint64_t WebRTCPlugin::StatsMemberGetUnsignedLong(const RTCStatsMemberInterface* member) { return 0; }

double WebRTCPlugin::StatsMemberGetDouble(const RTCStatsMemberInterface* member) { return 0; }

const char* WebRTCPlugin::StatsMemberGetString(const RTCStatsMemberInterface* member) { return nullptr; }

bool* WebRTCPlugin::StatsMemberGetBoolArray(const RTCStatsMemberInterface* member, size_t* length) { return nullptr; }

int32_t* WebRTCPlugin::StatsMemberGetIntArray(const RTCStatsMemberInterface* member, size_t* length) { return nullptr; }

uint32_t* WebRTCPlugin::StatsMemberGetUnsignedIntArray(const RTCStatsMemberInterface* member, size_t* length)
{
    return nullptr;
}

int64_t* WebRTCPlugin::StatsMemberGetLongArray(const RTCStatsMemberInterface* member, size_t* length)
{
    return nullptr;
}

uint64_t* WebRTCPlugin::StatsMemberGetUnsignedLongArray(const RTCStatsMemberInterface* member, size_t* length)
{
    return nullptr;
}

void WebRTCPlugin::PeerConnectionRegisterOnIceCandidate(PeerConnectionObject* obj, DelegateIceCandidate callback)
{
    obj->RegisterIceCandidate(callback);
}

CreateSessionDescriptionObserverX* WebRTCPlugin::PeerConnectionCreateOffer(
    Context* context, PeerConnectionObject* obj, const RTCOfferAnswerOptions* options)
{
    auto observer = CreateSessionDescriptionObserverX::Create(obj);
    obj->CreateOffer(*options, observer.get());
    return observer.get();
}

CreateSessionDescriptionObserverX* WebRTCPlugin::PeerConnectionCreateAnswer(
    Context* context, PeerConnectionObject* obj, const RTCOfferAnswerOptions* options)
{
    auto observer = CreateSessionDescriptionObserverX::Create(obj);
    obj->CreateAnswer(*options, observer.get());
    return observer.get();
}

PeerConnectionInterface::SignalingState WebRTCPlugin::PeerConnectionSignalingState(PeerConnectionObject* obj)
{
    return obj->connection->signaling_state();
}

RTCErrorType
WebRTCPlugin::PeerConnectionAddIceCandidate(PeerConnectionObject* obj, char* candidate, char* sdpMLineIndex, int sdpMid)
{
    SdpParseError error;
    std::unique_ptr<webrtc::IceCandidateInterface> _candidate(
        CreateIceCandidate(sdpMLineIndex, sdpMid, candidate, &error));
    if (!_candidate.get())
    {
        DebugError("Can't parse received candidate message. SdpParseError was: %s", error.description.c_str());
        return RTCErrorType::INVALID_PARAMETER;
    }
    if (!obj->connection->AddIceCandidate(_candidate.get()))
    {
        DebugError("Failed to apply the received candidate: %s", error.description.c_str());
        return RTCErrorType::UNSUPPORTED_OPERATION;
    }
    return RTCErrorType::NONE;
}

double* WebRTCPlugin::StatsMemberGetDoubleArray(const RTCStatsMemberInterface* member, size_t* length)
{
    return nullptr;
}

const char** WebRTCPlugin::StatsMemberGetStringArray(const RTCStatsMemberInterface* member, size_t* length)
{
    return nullptr;
}

const char**
WebRTCPlugin::StatsMemberGetMapStringUint64(const RTCStatsMemberInterface* member, uint64_t** values, size_t* length)
{
    return nullptr;
}

const char**
WebRTCPlugin::StatsMemberGetMapStringDouble(const RTCStatsMemberInterface* member, double** values, size_t* length)
{
    return nullptr;
}

RTCStatsMemberInterface::Type WebRTCPlugin::StatsMemberGetType(const RTCStatsMemberInterface* member)
{
    return RTCStatsMemberInterface::kSequenceDouble;
}

SetLocalDescriptionObserver*
WebRTCPlugin::PeerConnectionSetLocalDescription(PeerConnectionObject* obj, const RTCSessionDescription* desc)
{
    std::string error_;
    auto observer = SetLocalDescriptionObserver::Create(obj);
    auto errorType = obj->SetLocalDescription(*desc, observer, error_);

    if (errorType != RTCErrorType::NONE)
    {
        DebugError("PeerConnectionSetLocalDescription: %s", error_.c_str());
    }

    return observer.get();
}

SetLocalDescriptionObserver* WebRTCPlugin::PeerConnectionSetLocalDescriptionWithoutDescription(
    PeerConnectionObject* obj, RTCErrorType* errorType, char** error)
{
    return nullptr;
}

SetRemoteDescriptionObserver*
WebRTCPlugin::PeerConnectionSetRemoteDescription(PeerConnectionObject* obj, const RTCSessionDescription* desc)
{
    std::string error_;
    auto observer = SetRemoteDescriptionObserver::Create(obj);
    auto errorType = obj->SetRemoteDescription(*desc, observer, error_);
    if (errorType != RTCErrorType::NONE)
    {
        DebugError("PeerConnectionSetRemoteDescription: %s", error_.c_str());
    }
    return observer.get();
}

bool WebRTCPlugin::PeerConnectionGetLocalDescription(PeerConnectionObject* obj, RTCSessionDescription* desc)
{
    return false;
}

bool WebRTCPlugin::PeerConnectionGetRemoteDescription(PeerConnectionObject* obj, RTCSessionDescription* desc)
{
    return false;
}

bool WebRTCPlugin::PeerConnectionGetPendingLocalDescription(PeerConnectionObject* obj, RTCSessionDescription* desc)
{
    return false;
}

bool WebRTCPlugin::PeerConnectionGetPendingRemoteDescription(PeerConnectionObject* obj, RTCSessionDescription* desc)
{
    return false;
}

bool WebRTCPlugin::PeerConnectionGetCurrentLocalDescription(PeerConnectionObject* obj, RTCSessionDescription* desc)
{
    return false;
}

bool WebRTCPlugin::PeerConnectionGetCurrentRemoteDescription(PeerConnectionObject* obj, RTCSessionDescription* desc)
{
    return false;
}

RtpReceiverInterface**
WebRTCPlugin::PeerConnectionGetReceivers(Context* context, PeerConnectionObject* obj, size_t* length)
{
    return nullptr;
}

RtpSenderInterface** WebRTCPlugin::PeerConnectionGetSenders(Context* context, PeerConnectionObject* obj, size_t* length)
{
    return nullptr;
}

RtpTransceiverInterface**
WebRTCPlugin::PeerConnectionGetTransceivers(Context* context, PeerConnectionObject* obj, size_t* length)
{
    return nullptr;
}

DataChannelInterface* WebRTCPlugin::ContextCreateDataChannel(
    Context* ctx, PeerConnectionObject* obj, const char* label, const RTCDataChannelInit* options)
{
    DataChannelInit _options;
    _options.ordered = options->ordered.value_or(true);
    _options.maxRetransmitTime = static_cast<absl::optional<int32_t>>(options->maxRetransmitTime);
    _options.maxRetransmits = static_cast<absl::optional<int32_t>>(options->maxRetransmits);
    _options.protocol = options->protocol == nullptr ? "" : options->protocol;
    _options.negotiated = options->negotiated.value_or(false);
    _options.id = options->id.value_or(-1);

    return ctx->CreateDataChannel(obj, label, _options);
}

void WebRTCPlugin::ContextDeleteDataChannel(Context* ctx, DataChannelInterface* channel) { }

void WebRTCPlugin::PeerConnectionRegisterIceConnectionChange(
    PeerConnectionObject* obj, DelegateOnIceConnectionChange callback)
{
}

void WebRTCPlugin::PeerConnectionRegisterIceGatheringChange(
    PeerConnectionObject* obj, DelegateOnIceGatheringChange callback)
{
}

void WebRTCPlugin::PeerConnectionRegisterConnectionStateChange(
    PeerConnectionObject* obj, DelegateOnConnectionStateChange callback)
{
}

void WebRTCPlugin::StatsCollectorRegisterCallback(DelegateCollectStats callback) { }

void WebRTCPlugin::CreateSessionDescriptionObserverRegisterCallback(DelegateCreateSessionDesc callback)
{
    CreateSessionDescriptionObserverX::RegisterCallback(callback);
}

void WebRTCPlugin::SetLocalDescriptionObserverRegisterCallback(DelegateSetLocalDesc callback)
{
    SetLocalDescriptionObserver::RegisterCallback(callback);
}

void WebRTCPlugin::SetRemoteDescriptionObserverRegisterCallback(DelegateSetRemoteDesc callback)
{
    SetRemoteDescriptionObserver::RegisterCallback(callback);
}
}