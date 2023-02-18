#include "API.h"
#include "Context.h"
#include "WebRTCPlugin.h"

using namespace webrtc;

WebRTCPlugin* plugin;
Context* ctx;

void API::Print(){
    std::cout << "What" << std::endl;
}

void API::StartServer(){
    plugin = new WebRTCPlugin;
    ctx = new Context;

    PeerConnectionObject* pco = plugin->ContextCreatePeerConnection(ctx);
    printf("WebRTCPlugin created!!\n");

    delete plugin;
    delete ctx;
}