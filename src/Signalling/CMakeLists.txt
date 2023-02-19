add_library(Signalling STATIC)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

target_sources(Signalling
        PRIVATE
        Signalling.cpp
        Signalling.h)

target_include_directories(Signalling SYSTEM PRIVATE websocketpp)
target_include_directories(Signalling SYSTEM PRIVATE .)