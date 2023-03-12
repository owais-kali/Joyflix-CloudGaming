option(OBS_SRC "Set Obs-Studio rc path by -DOBS_SRC_SRC")

if(NOT OBS_SRC)
    message( SEND_ERROR "Set Obs-Studio src path by -DOBS_SRC" )
endif()

add_executable(OBS)

target_sources(OBS
        PRIVATE
        OBS.cpp
        OBS.h)

target_include_directories(
        OBS
        PRIVATE
        ${OBS_SRC}
        ${OBS_SRC}/deps/libcaption/)

target_link_libraries(OBS
        PRIVATE
        /media/owais/53f67e15-4b5a-405d-a681-bffb9437ee41/CloudGaming/obs-studio/cmake-build-debug/libobs/libobs.so
        )