//
// Created by owais on 25/8/22.
//
#pragma clang diagnostic ignored "-Wunused-variable"

#include "WebRTCPlugin.h"

#include "Context.h"
#include "SSDO.h"
#include "Logger.h"
#include "api/jsep.h"

namespace joyflix {
    namespace webrtc {
        PeerConnectionObject *WebRTCPlugin::_ContextCreatePeerConnection(
                Context *context,
                const ::webrtc::PeerConnectionInterface::RTCConfiguration &config) {
            const auto obj = context->CreatePeerConnection(config);
            if (obj == nullptr)
                return nullptr;
            const auto observer = SSDO::Create(obj);
            context->AddObserver(obj->connection.get(), observer);
            return obj;
        }

        PeerConnectionObject *WebRTCPlugin::ContextCreatePeerConnection(
                Context *context) {
            ::webrtc::PeerConnectionInterface::RTCConfiguration config = {};
            config.sdp_semantics = SdpSemantics::kUnifiedPlan;
            config.enable_implicit_rollback = true;
            webrtc::PeerConnectionInterface::IceServer server;
            server.uri = GetPeerConnectionString();
            config.servers.push_back(server);
            return _ContextCreatePeerConnection(context, config);
        }

        void WebRTCPlugin::PeerConnectionRegisterCallbackCreateSD(
                PeerConnectionObject *obj,
                DelegateCreateSDSuccess onSuccess,
                DelegateCreateSDFailure onFailure) {
            obj->RegisterCallbackCreateSD(onSuccess, onFailure);
        }

        void WebRTCPlugin::PeerConnectionRegisterOnIceCandidate(
                PeerConnectionObject *obj,
                DelegateIceCandidate callback) {
            obj->RegisterIceCandidate(callback);
        }

        void WebRTCPlugin::PeerConnectionCreateOffer(
                PeerConnectionObject *obj,
                const RTCOfferAnswerOptions *options) {
            obj->CreateOffer(*options);
        }

        void WebRTCPlugin::PeerConnectionCreateAnswer(
                PeerConnectionObject *obj,
                const RTCOfferAnswerOptions *options) {
            obj->CreateAnswer(*options);
        }

        RTCErrorType WebRTCPlugin::PeerConnectionSetLocalDescription(
                Context *context,
                PeerConnectionObject *obj,
                const RTCSessionDescription *desc,
                std::string &error) {
            RTCErrorType errorType = obj->SetLocalDescription(
                    *desc, context->GetObserver(obj->connection.get()), error);

            return errorType;
        }

        RTCErrorType WebRTCPlugin::PeerConnectionSetRemoteDescription(
                Context *context,
                PeerConnectionObject *obj,
                const RTCSessionDescription *desc,
                std::string &error) {
            RTCErrorType errorType = obj->SetRemoteDescription(
                    *desc, context->GetObserver(obj->connection.get()), error);
            return errorType;
        }

        ::webrtc::PeerConnectionInterface::SignalingState
        WebRTCPlugin::PeerConnectionSignalingState(PeerConnectionObject *obj) {
            return obj->connection->signaling_state();
        }

        RTCErrorType
        WebRTCPlugin::PeerConnectionAddIceCandidate(PeerConnectionObject *obj, char *candidate, char *sdpMLineIndex,
                                                    int sdpMid) {
            SdpParseError error;
            std::unique_ptr<webrtc::IceCandidateInterface> _candidate(
                    CreateIceCandidate(sdpMLineIndex, sdpMid, candidate, &error));
            if (!_candidate.get()) {
                DebugError("Can't parse received candidate message. SdpParseError was: %s", error.description.c_str());
                return RTCErrorType::INTERNAL_ERROR;
            }
            if (!obj->connection->AddIceCandidate(_candidate.get())) {
                DebugError("Failed to apply the received candidate: %s", error.description.c_str());
            }
        }

        char *ConvertString(const std::string str) {
            const size_t size = str.size();
            char *ret = static_cast<char *>(CoTaskMemAlloc(size + sizeof(char)));
            str.copy(ret, size);
            ret[size] = '\0';
            return ret;
        }

        void WebRTCPlugin::AddTracks(Context *context) {
            context->AddTracks();
        }
    }
}