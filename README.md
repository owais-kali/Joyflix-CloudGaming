# JOYFLIX: Unleash the Power of Cloud Gaming

![Logo](https://raw.githubusercontent.com/joyflix99/Joyflix-CloudGaming/main/Logo.png)

## :warning: <b>This project is under development and nowhere near ready for use.</b> <br>

## Architecture: Where Innovation Meets Gaming
![Architecture](https://raw.githubusercontent.com/joyflix99/Joyflix-CloudGaming/a16237d2730549d3112416a3057476715f069927/Joyflix%20Engine.drawio.png)

## Introduction: Redefining the Gaming Experience
Welcome to Joyflix, the ultimate cloud gaming platform that brings your favorite games to life. Harnessing the power of cutting-edge technologies, Joyflix renders games on remote servers and streams them directly to your browser, just like Google Stadia. With Joyflix, you can play any game on any platform, whether it's on your desktop, Android device, or iOS device, providing a seamless and immersive gaming experience.

## Game Capture: Breaking the Barriers of Traditional Approaches
Unlike most cloud gaming software that rely on screen capture or the creation of X11 windows for Linux, Joyflix takes a revolutionary approach to game capture. We eliminate the inefficiencies and limitations of these methods by running games in a completely headless manner, without any windowing system. To achieve this, we have developed a custom version of Wine that supports headless operation. By doing so, we ensure optimal performance, efficient containerization using Docker, and the ability to run multiple games simultaneously. Explore the source code of our headless Wine version [here](https://github.com/owais-kali/wine-headless) 

## Encoding: Powering Seamless Video Streaming
At Joyflix, we understand the importance of smooth and high-quality video streaming. That's why we leverage Nvidia's NVENC API for hardware encoding. By utilizing the power of Nvidia GPUs with NVENC support, we ensure superior video encoding capabilities. To check if your server's GPU supports NVENC, refer to the [NVENC support matrix](https://developer.nvidia.com/nvidia-video-codec-sdk)

## Webrtc: The Foundation of Seamless Communication
Joyflix relies on Google's WebRTC implementation, a cutting-edge technology that enables real-time communication and streaming between the server and client. With WebRTC, we deliver an optimized and reliable streaming experience, ensuring minimal latency and optimal video and input synchronization. Learn more about WebRTC [here](https://webrtc.googlesource.com/src/)

## Handling user Input: Personalized Control at Your Fingertips
With the absence of an X server in our headless environment, we faced the challenge of handling user input effectively. To overcome this, we have customized the source code of Linux Wine, enabling seamless and accurate handling of user input. This ensures that you have full control over your gaming experience, right from your browser.

## Client: Boundless Gaming on Your Terms
Thanks to the power of WebRTC, Joyflix is accessible on any device that supports the Chrome browser. Whether you're gaming on your desktop, Android device, or iOS device, Joyflix provides a consistent and exhilarating gaming experience, tailored to your preferences.

## Game Saves: Your Progress, Always Secure
Say goodbye to worries about losing your game progress. At Joyflix, we take game saves seriously. We provide automatic backup and storage of your game saves on our secure servers, eliminating the need to rely on platforms like Steam or Epic Games for backup. Enjoy peace of mind as you embark on your gaming adventures, knowing that your progress is always safeguarded.

Join us on this exciting journey as we revolutionize cloud gaming with Joyflix. Be sure to stay tuned for updates and the official release of our platform. Together, let's redefine the way we play and experience games!

<!-- Roadmap -->
## :compass: Roadmap

* [x] Create VulkanApp
* [x] Create CudaApp
* [ ] Create Nvenc Encoder
* [x] Add WebRTC(C++) and Signalling server
* [x] Create simple webapp using nodejs
* [x] Exchange offer & answer and Connect WebRTC(C++) to webapp 
* [ ] Stream Encoded video to web browser using webrtc 
* [ ] Send input data from webapp to WebRTC(C++) using DataChannel
* [ ] Integrate this code to wine-headless repo

## Libraries
### [WebRTC](https://webrtc.googlesource.com/src/)
### [com.unity.webrtc](https://github.com/Unity-Technologies/com.unity.webrtc)  [Modified Source to create a wrapper for Google's Webrtc Source]
### [websocketpp](https://github.com/zaphoyd/websocketpp)
### [nlohmann/json](https://github.com/nlohmann/json)
