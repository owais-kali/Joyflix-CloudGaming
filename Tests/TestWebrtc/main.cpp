#include <api/scoped_refptr.h>
#include <api/create_peerconnection_factory.h>
#include <iostream>

int main(int argc, char* argv[]) {
    printf("hello world!\n");
    auto m_peerConnectionFactory = webrtc::CreatePeerConnectionFactory(
        nullptr,
       nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr);
    return 0;
}