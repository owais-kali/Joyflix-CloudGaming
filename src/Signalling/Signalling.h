#pragma once
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include "../pch.h"

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

class Signalling {
public:
    typedef std::function<void (std::string)> DelegateOnMessage;
private:
    typedef websocketpp::server<websocketpp::config::asio> server;
    // pull out the type of messages sent by our config
    typedef server::message_ptr message_ptr;
    DelegateOnMessage onMessageDelegate;

    // Create a server endpoint
    server signalling_server;
    std::thread* signalling_server_thread;

    bool IsClientConnected = false;
    websocketpp::connection_hdl client_connection_hdl;

    server::connection_ptr client_connection_ptr;

    void OnConnect(server* s, websocketpp::connection_hdl hdl);
    void OnDisconnect(server* s, websocketpp::connection_hdl hdl);
    void OnMessage(server* s, websocketpp::connection_hdl hdl, message_ptr msg);
public:
    Signalling(DelegateOnMessage onMessageCallback);
    void StartServer(int port);
    void SendMessage(std::string& msg);
    void StopServer();
};