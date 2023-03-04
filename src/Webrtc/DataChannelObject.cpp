//
// Created by owais on 28/2/23.
//

#include "DataChannelObject.h"
#include "Logger.h"

webrtc::DataChannelObject::DataChannelObject(
    rtc::scoped_refptr<DataChannelInterface> channel, webrtc::PeerConnectionObject& pc)
    : dataChannel(channel)
{
    dataChannel->RegisterObserver(this);
    DebugLog("DataChannelObject Created!\n");
}

webrtc::DataChannelObject::~DataChannelObject() {
    dataChannel->UnregisterObserver();
    DebugLog("DataChannelObject Destroyed!\n");
}

void webrtc::DataChannelObject::OnStateChange() {
    auto state = dataChannel->state();
    switch (state)
    {
    case webrtc::DataChannelInterface::kOpen:
        if (onOpen != nullptr)
        {
            onOpen(this->dataChannel.get());
        }
        break;
    case webrtc::DataChannelInterface::kClosed:
        if (onClose != nullptr)
        {
            onClose(this->dataChannel.get());
        }
        break;
    case webrtc::DataChannelInterface::kConnecting:
    case webrtc::DataChannelInterface::kClosing:
        break;
    }
}

void webrtc::DataChannelObject::OnMessage(const webrtc::DataBuffer& buffer) {
    DebugLog("DataChannel: Got message:\n");
}
