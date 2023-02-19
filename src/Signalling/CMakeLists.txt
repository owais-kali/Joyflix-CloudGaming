add_executable(Server)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
target_sources(Server
        PRIVATE
        Server.cpp
        Server.h)

target_include_directories(Server SYSTEM PRIVATE websocketpp)
target_include_directories(Server SYSTEM PRIVATE .)