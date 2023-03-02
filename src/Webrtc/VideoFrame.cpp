#include "VideoFrameX.h"

namespace webrtc
{
GpuMemoryBufferInterface* webrtc::VideoFrameX::GetGpuMemoryBuffer() const { return nullptr; }
bool webrtc::VideoFrameX::HasGpuMemoryBuffer() const { return false; }
rtc::scoped_refptr<VideoFrameX> webrtc::VideoFrameX::WrapExternalGpuMemoryBuffer(
    const webrtc::Size& size,
    rtc::scoped_refptr<GpuMemoryBufferInterface> gpu_memory_buffer,
    webrtc::VideoFrameX::ReturnBufferToPoolCallback returnBufferToPoolCallback,
    webrtc::TimeDelta timestamp)
{
    return rtc::scoped_refptr<VideoFrameX>();
}
VideoFrameX::VideoFrameX(
    const Size& size,
    rtc::scoped_refptr<GpuMemoryBufferInterface> buffer,
    VideoFrameX::ReturnBufferToPoolCallback returnBufferToPoolCallback,
    TimeDelta timestamp)
    : size_(size)
    , gpu_memory_buffer_(std::move(buffer))
    , returnBufferToPoolCallback_(returnBufferToPoolCallback)
    , timestamp_(timestamp)
{
}
VideoFrameX::~VideoFrameX() { }
}