#pragma once
#pragma clang diagnostic ignored "-Wunused-variable" 

#pragma region webRTC related
#include "api/media_stream_interface.h"
#include "api/peer_connection_interface.h"
#include "api/create_peerconnection_factory.h"
#include "api/video_codecs/video_decoder_factory.h"
#include "api/video_codecs/builtin_video_decoder_factory.h"
#include "api/video_codecs/video_decoder.h"
#include "api/video_codecs/video_encoder_factory.h"
#include "api/video_codecs/builtin_video_encoder_factory.h"
#include "api/video_codecs/video_encoder.h"
#include "api/video_codecs/sdp_video_format.h"
#include "api/video/video_frame.h"
#include "api/video/video_frame_buffer.h"
#include "api/video/video_sink_interface.h"
#include "api/video/i420_buffer.h"
#include "pc/video_track_source_proxy.h"
#include "pc/video_track_source.h"

#include "rtc_base/thread.h"
#include "rtc_base/ref_counted_object.h"
#include "rtc_base/strings/json.h"
#include "rtc_base/logging.h"
#include "rtc_base/checks.h"
#include "rtc_base/ssl_adapter.h"
#include "rtc_base/arraysize.h"
#include "rtc_base/net_helpers.h"
#include "rtc_base/string_utils.h"
#include "rtc_base/physical_socket_server.h"
#include "rtc_base/third_party/sigslot/sigslot.h"
#include "rtc_base/async_tcp_socket.h"

#include "test/vcm_capturer.h"
#include "api/jsep.h"

#ifdef _WIN32
#include "rtc_base/win32.h"
#include "rtc_base/win32_socket_server.h"
#include "rtc_base/win32_socket_init.h"
#include "rtc_base/win32_socket_server.h"
#endif

#include "media/engine/internal_encoder_factory.h"
#include "media/engine/internal_decoder_factory.h"
#include "api/video_codecs/h264_profile_level_id.h"
#include "media/base/adapted_video_track_source.h"
#include "media/base/media_channel.h"
#include "media/base/video_common.h"
#include "media/base/video_broadcaster.h"

#include "modules/video_capture/video_capture_impl.h"
#include "modules/video_capture/video_capture_factory.h"
#include "modules/video_coding/codecs/h264/include/h264.h"
#include "modules/video_coding/codecs/vp8/include/vp8.h"
#include "modules/video_coding/codecs/vp9/include/vp9.h"

#include "common_video/h264/h264_bitstream_parser.h"
#include "common_video/h264/h264_common.h"
#include "common_video/include/bitrate_adjuster.h"

#include "pc/media_stream_observer.h"
#include "pc/local_audio_source.h"

#pragma endregion

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wkeyword-macro"
#pragma clang diagnostic pop

namespace webrtc
{
#define CoTaskMemAlloc(p) malloc(p)
#define CoTaskMemFree(p) free(p)

    using byte = unsigned char;
    using uint8 = unsigned char;
    using uint16 = unsigned short int;
    using uint32 = unsigned int;
    using uint64 = unsigned long long;
    using int8 = signed char;
    using int16 = signed short int;
    using int32 = signed int;
    using int64 = signed long long;

    const uint32 bufferedFrameNum = 3;
} // end namespace webrtc
