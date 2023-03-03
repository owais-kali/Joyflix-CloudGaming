namespace webrtc
{
struct RTCOfferAnswerOptions
{
    bool iceRestart;
    bool voiceActivityDetection;
};

enum class RTCSdpType
{
    Offer,
    PrAnswer,
    Answer,
    Rollback
};

struct RTCSessionDescription
{
    RTCSdpType type;
    const char* sdp;
};

}