#include "API.h"
#include "Context.h"
#include "WebRTCPlugin.h"
#include "Logger.h"

using namespace webrtc;

WebRTCPlugin* plugin;

API::API() {
    plugin = new WebRTCPlugin;
    DebugLog("WebRTCPlugin Created!");
}

API::~API() {
    DebugLog("WebRTCPlugin Destroyed!");
    delete plugin;
}

void* API::ContextCreate() {
    Context* ctx = new Context;
    return ctx;
}

void API::ContextDestroy(void *ctx) {
    delete ctx;
}

