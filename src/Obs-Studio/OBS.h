#ifndef OBS_H
#define OBS_H

#include <iostream>
#include <unistd.h>
#include "libobs/obs.hpp"
#include "libobs/obs-nix-platform.h"
#include "libobs/obs-internal.h"

extern "C" struct obs_core *GetOBSCore();

class OBS {
public:
    OBS();
    ~OBS();

    void start();

private:
    void obs_add_module_path();
    void obs_load_all_modules2();
    void start_encoding(obs_source_t *source);

    obs_source_t *source_;
};

#endif // OBS_H
