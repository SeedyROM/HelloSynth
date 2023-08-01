#pragma once

#include <functional>
#include <map>

using Callback = std::function<void(float)>;

class Transport
{
public:
    enum class State
    {
        Stopping,
        Stopped,
        Starting,
        Started,
        Looping,
    };

    State getState() const;
    float getPlaybackPosition() const;
    void setPlaybackPosition(float playbackPosition);
    float getLoopLength() const;
    void setLoopLength(float loopLength);

    void play();
    void stop();

    void onChange(State state, Callback callback);
    void tick(float dt);

    static const char *getStateName(State state);

private:
    State m_state = State::Stopped;
    State m_lastState = State::Stopped;
    float m_playbackPosition = 0;
    float m_loopLength = 0;
    std::map<State, Callback> m_callbacks;

    void setState(State state);
    void callCallback(State state);
};