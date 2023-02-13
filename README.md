# Joyflix-CloudGaming

Open-source Cloud Gaming Service

![Design](https://github.com/joyflix99/Joyflix-CloudGaming/blob/cuda_app/Joyflix%20Engine.drawio.png)

## Introduction
Joyflix provides a cloud gaming platform to run Windows OS games on linux using [Wine](https://www.winehq.org/).

In cloud gaming, games are run on remote servers and media are streamed to the player optimally to ensure
the most comfortable user interaction. It opens the ability to play any  games on web-browser directly, which are
fully compatible with multi-platform like Desktop, Android, IOS.

## Game Capture
- Unlike most cloud gaming software that use screen capture. Or in linux case we create an X11 window and then capture the window.
- This is very inefficient and also hard to containerize using docker as either the X11 server needs to be run on host computer to get hardware acceleration and you can only run one game per x11 server;
- Or you can run a virtual framebuffer inside a docker container using xvfb but this is very slow cause it uses CPU, RAM instead of GPU. Example of such a software is [
  selkies-gstreamer](https://github.com/selkies-project/selkies-gstreamer)
- So we have decided to completely remove any kind of windowing system and run it complete headless. This is challenging cause Wines original source code does not have a headless mode. So we have to make our own custom version of wine that can be run headless. 
- You can check out our headless wine source code from [here](https://github.com/joyflix99/wine-tkg-headless) 

## Encoding
- For Encoding we use Nvidia's NVENC API for hardware encoding. 
- So the server must have an nvidia gpu with nvenc support. See [nvenc support matrix](https://developer.nvidia.com/nvidia-video-codec-sdk)

## Webrtc 
- We use googles Webrtc implementation see [here](https://webrtc.googlesource.com/src/)

## Handling user Input
- Again since there is no X server we have to customize linux wine's source code to handle user input. 

## Client
- Since it is built on Webrtc, Joyflix can be run on any device that supports Chrome browser. 

## Game Saves
- Game Saves are backed up on Joyflix server so need to run steam or epic games to back up saves
