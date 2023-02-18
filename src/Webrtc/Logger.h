#pragma once

namespace webrtc {
    void LogPrint(const char *fmt, ...);

    void LogPrint(const wchar_t *fmt, ...);

    void LogPrintAsync(int delay, const char *fmt, ...);

#define DebugLog(...) webrtc::LogPrint("EngineServer Log: " __VA_ARGS__)
#define DebugWarning(...) webrtc::LogPrint("EngineServer Warning: " __VA_ARGS__)
#define DebugError(...) webrtc::LogPrint("EngineServer Error: " __VA_ARGS__)
#define DebugLogW(...) webrtc::LogPrint(L"EngineServer Log: " __VA_ARGS__)
#define DebugWarningW(...) webrtc::LogPrint(L"EngineServer Warning: " __VA_ARGS__)
#define DebugErrorW(...) webrtc::LogPrint(L"EngineServer Error: " __VA_ARGS__)
#define NV_RESULT(NvFunction) NvFunction == NV_ENC_SUCCESS
}
