#pragma once
class Signalling {
public:
    void StartServer(int port);
    void SendMessage(std::string& msg);
    void StopServer();
};