#pragma once

#include <api/units/time_delta.h>
#include <rtc_base/ref_counted_object.h>
#include <rtc_base/timestamp_aligner.h>

//#include "GpuMemoryBuffer.h"
#include "Size.h"

namespace webrtc
{
    class GpuMemoryBufferInterface
    : public rtc::RefCountInterface
    {
    public:
        virtual Size GetSize() const = 0;
    protected:
        ~GpuMemoryBufferInterface() override = default;
    };
    class UnityRenderingExtTextureFormat;

    class VideoFrameX : public rtc::RefCountInterface
    {
    public:
        using ReturnBufferToPoolCallback = std::function<void(rtc::scoped_refptr<GpuMemoryBufferInterface>)>;

        VideoFrameX() = delete;
        VideoFrameX(const VideoFrameX&) = delete;
        VideoFrameX& operator=(const VideoFrameX&) = delete;

        Size size() const { return size_; }

        TimeDelta timestamp() const { return timestamp_; }
        void set_timestamp(TimeDelta timestamp) { timestamp_ = timestamp; }

        GpuMemoryBufferInterface* GetGpuMemoryBuffer() const;
        bool HasGpuMemoryBuffer() const;

        static rtc::scoped_refptr<VideoFrameX> WrapExternalGpuMemoryBuffer(
            const Size& size,
            rtc::scoped_refptr<GpuMemoryBufferInterface> gpu_memory_buffer,
            ReturnBufferToPoolCallback returnBufferToPoolCallback,
            TimeDelta timestamp);

    protected:
        VideoFrameX(
            const Size& size,
            rtc::scoped_refptr<GpuMemoryBufferInterface> buffer,
            ReturnBufferToPoolCallback returnBufferToPoolCallback,
            TimeDelta timestamp);
        virtual ~VideoFrameX() override;

    private:
        Size size_;
        rtc::scoped_refptr<GpuMemoryBufferInterface> gpu_memory_buffer_;
        ReturnBufferToPoolCallback returnBufferToPoolCallback_;
        TimeDelta timestamp_;
    };
}
