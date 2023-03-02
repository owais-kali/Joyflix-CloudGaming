//
// Created by owais on 28/2/23.
//

#include "DataChannelObject.h"

webrtc::DataChannelObject::DataChannelObject(
    rtc::scoped_refptr<DataChannelInterface> channel, webrtc::PeerConnectionObject& pc)
{
}

webrtc::DataChannelObject::~DataChannelObject() { }

void webrtc::DataChannelObject::OnStateChange() { }

void webrtc::DataChannelObject::OnMessage(const webrtc::DataBuffer& buffer) { }
