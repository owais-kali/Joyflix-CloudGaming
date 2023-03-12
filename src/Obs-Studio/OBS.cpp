#include <iostream>
#include <unistd.h>
#include "OBS.h"
#include "libobs/obs.hpp"
#include "libobs/obs-nix-platform.h"
#include "libobs/obs-internal.h"

extern "C" struct obs_core *GetOBSCore();

void obs_add_module_path(){
    obs_core* obs = GetOBSCore();

    struct obs_module_path omp;
    omp.bin = bstrdup("/media/owais/53f67e15-4b5a-405d-a681-bffb9437ee41/CloudGaming/obs-studio/cmake-build-debug/rundir/Debug/obs-plugins/64bit/");
    omp.data = bstrdup("/media/owais/53f67e15-4b5a-405d-a681-bffb9437ee41/CloudGaming/obs-studio/cmake-build-debug/rundir/Debug/data/obs-plugins/%module%");
    da_push_back(obs->module_paths, &omp);
}

void obs_load_all_modules2(){
    struct obs_module_failure_info mfi;
    obs_load_all_modules2(&mfi);
}

void StartupOBS(){
    obs_set_nix_platform(obs_nix_platform_type::OBS_NIX_PLATFORM_X11_EGL);

    obs_startup("en-GB", "../../config/obs-studio/plugin_config", nullptr);
    obs_add_module_path();
    obs_load_all_modules2();

    OBSSourceAutoRelease source = obs_get_source_by_name("wine");
    source = obs_source_create("xcomposite_input", "Window Capture (Xcomposite)", NULL, nullptr);


}

int main(){
    StartupOBS();
    sleep(100);
    printf("hello world\n");
}