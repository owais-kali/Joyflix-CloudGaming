#pragma once

#include <mutex>

#include <absl/types/optional.h>
#include <api/media_stream_interface.h>
#include <api/task_queue/task_queue_factory.h>
#include <media/base/adapted_video_track_source.h>
#include <rtc_base/task_queue.h>

#include "VideoFrameX.h"

namespace webrtc
{

    using namespace ::webrtc;

    // This class implements webrtc's VideoTrackSourceInterface. To pass frames down
    // the webrtc video pipeline, each received a media::VideoFrame is converted to
    // a webrtc::VideoFrame, taking any adaptation requested by downstream classes
    // into account.
    class VideoFrameScheduler;
    class VideoTrackSource : public rtc::AdaptedVideoTrackSource
    {
    public:
        struct FrameAdaptationParams
        {
            bool should_drop_frame;
            int crop_x;
            int crop_y;
            int crop_width;
            int crop_height;
            int scale_to_width;
            int scale_to_height;
        };

        VideoTrackSource(
            bool is_screencast, absl::optional<bool> needs_denoising, TaskQueueFactory* taskQueueFactory);
        ~VideoTrackSource() override;
        // void SetState(SourceState state);
        SourceState state() const override;

        bool remote() const override;
        bool is_screencast() const override;
        absl::optional<bool> needs_denoising() const override;
        void OnFrameCaptured(rtc::scoped_refptr<VideoFrameX> frame);

        using VideoTrackSourceInterface::AddOrUpdateSink;
        using VideoTrackSourceInterface::RemoveSink;

        static rtc::scoped_refptr<VideoTrackSource>
        Create(bool is_screencast, absl::optional<bool> needs_denoising, TaskQueueFactory* taskQueueFactory);

    private:
        void CaptureNextFrame();
        void SendFeedback();
        FrameAdaptationParams ComputeAdaptationParams(int width, int height, int64_t time_us);

        rtc::TimestampAligner timestamp_aligner_;

        const bool is_screencast_;
        const absl::optional<bool> needs_denoising_;
        std::mutex mutex_;

        std::unique_ptr<rtc::TaskQueue> taskQueue_;
        std::unique_ptr<VideoFrameScheduler> scheduler_;
        rtc::scoped_refptr<webrtc::VideoFrameX> frame_;
    };

} // end namespace webrtc
