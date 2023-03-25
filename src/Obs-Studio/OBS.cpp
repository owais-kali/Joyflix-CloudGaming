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

void start_encoding(obs_source_t *source){
    // Create an encoder for the captured window
    obs_encoder_t *encoder = obs_video_encoder_create("obs_x264", "H264 encoder", NULL, nullptr);

    // Configure the encoder settings
    obs_data_t *settings = obs_encoder_get_settings(encoder);
    obs_data_set_int(settings, "bitrate", 2500);
    obs_data_set_string(settings, "preset", "medium");
    obs_data_set_string(settings, "tune", "film");
    obs_encoder_update(encoder, settings);
    obs_data_release(settings);

    // Create a new output for the encoded stream
    obs_output_t *output = obs_output_create("simple_output", "File Output", NULL, nullptr);

    // Configure the output settings
    settings = obs_output_get_settings(output);
    obs_data_set_string(settings, "path", "output.mp4");
    obs_output_update(output, settings);
    obs_data_release(settings);

    // Set the encoder and source for the output
    obs_output_set_video_encoder(output, encoder);
    obs_output_set_audio_encoder(output, nullptr);
    obs_output_set_audio_source(output, nullptr);
    obs_output_set_video_source(output, source);

    // Start the output
    obs_output_start(output);

    // Cleanup
    obs_encoder_release(encoder);
}

void StartupOBS(){
    obs_set_nix_platform(obs_nix_platform_type::OBS_NIX_PLATFORM_X11_EGL);

    obs_startup("en-GB", "../../config/obs-studio/plugin_config", nullptr);
    obs_add_module_path();
    obs_load_all_modules2();

    // Create a new source using XComposite input
    obs_source_t *source = obs_source_create("xcomposite_input", "Window Capture (Xcomposite)", NULL, nullptr);

    // Set the capture settings
    obs_data_t *settings = obs_source_get_settings(source);
    obs_data_set_bool(settings, "monitor_only", false);
    obs_data_set_bool(settings, "limit_captures", true);
    obs_data_set_string(settings, "window", "my_window_name");
    obs_data_set_int(settings, "fps", 60);
    obs_source_update(source, settings);
    obs_data_release(settings);

    // Add the source to the main scene
    obs_scene_t *scene = obs_get_scene_by_name("Scene");
    obs_sceneitem_t *item = obs_scene_add(scene, source);
    obs_sceneitem_set_name(item, "Window Capture");

    // Set the preview and program scene to the main scene
    obs_frontend_set_current_scene(scene, false);
    obs_frontend_set_current_scene(scene, true);

    // Start encoding the captured window
    start_encoding(source);

    // Wait for encoding to finish
    sleep(10);

    // Stop the output
    obs_output_t *output = obs_output_from_source(source);
    obs_output_stop(output);

    // Remove the source and output
    obs_sceneitem_remove(item);
    obs_output_release(output);

    // Cleanup
    obs_source_release(source);
    obs_scene_release(scene);
    obs_shutdown();


