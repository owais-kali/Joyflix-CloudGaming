#pragma once

namespace webrtc
{
void LogPrint(const char* fmt, ...);

void LogPrint(const wchar_t* fmt, ...);

void LogPrintAsync(int delay, const char* fmt, ...);

#define DebugLog(...) LogPrint("WebRTCServer Log: " __VA_ARGS__)
#define DebugWarning(...) LogPrint("WebRTCServer Warning: " __VA_ARGS__)
#define DebugError(...) LogPrint("WebRTCServer Error: " __VA_ARGS__)
#define DebugLogW(...) LogPrint("WebRTCServer Log: " __VA_ARGS__)
#define DebugWarningW(...) LogPrint("WebRTCServer Warning: " __VA_ARGS__)
#define DebugErrorW(...) LogPrint("WebRTCServer Error: " __VA_ARGS__)
#define NOT_IMPLEMENTED(...) LogPrint("WebRTCServer NOT IMPLEMENTED: " __VA_ARGS__)
#define NV_RESULT(NvFunction) NvFunction == NV_ENC_SUCCESS
}