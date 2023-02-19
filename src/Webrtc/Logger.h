#pragma once

namespace webrtc {
    void LogPrint(const char *fmt, ...);

    void LogPrint(const wchar_t *fmt, ...);

    void LogPrintAsync(int delay, const char *fmt, ...);

#define DebugLog(...) webrtc::LogPrint("WebRTCServer Log: " __VA_ARGS__)
#define DebugWarning(...) webrtc::LogPrint("WebRTCServer Warning: " __VA_ARGS__)
#define DebugError(...) webrtc::LogPrint("WebRTCServer Error: " __VA_ARGS__)
#define DebugLogW(...) webrtc::LogPrint(L"WebRTCServer Log: " __VA_ARGS__)
#define DebugWarningW(...) webrtc::LogPrint(L"WebRTCServer Warning: " __VA_ARGS__)
#define DebugErrorW(...) webrtc::LogPrint(L"WebRTCServer Error: " __VA_ARGS__)
#define NV_RESULT(NvFunction) NvFunction == NV_ENC_SUCCESS
}
