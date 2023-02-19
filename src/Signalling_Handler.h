#pragma once
class Signalling_Handler {
    void StartSignalling(int port);
    void StopSignalling();
    void RegisterOnOfferCallback();
    void RegisterOnAnswerCallback();
    void RegisterOnICECallback();
};
