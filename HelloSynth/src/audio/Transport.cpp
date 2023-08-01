#include "Transport.h"

#include "utils/Logger.h"

inline Transport::State Transport::getState() const
{
    return m_state;
}

inline void Transport::setState(State state)
{
    // Check if the state is valid
    if (state < State::Stopped || state > State::Looping)
    {
        log::error("Can't set invalid state: %d", state);
        return;
    }

    // If the new state is not the same...
    m_state = state;

    // If the new state is the same as the current state, do nothing
    if (m_lastState == m_state)
    {
        return;
    }

    log::info("Transport state changed: %s", Transport::getStateName(m_state));

    // Call the callback for the given state
    callCallback(m_state);

    // Update the last state
    m_lastState = m_state;
}

float Transport::getPlaybackPosition() const
{
    return m_playbackPosition;
}

inline void Transport::setPlaybackPosition(float playbackPosition)
{
    m_playbackPosition = playbackPosition;
}

float Transport::getLoopLength() const
{
    return m_loopLength;
}

void Transport::setLoopLength(float loopLength)
{
    m_loopLength = loopLength;
}

void Transport::onChange(Transport::State state, Callback callback)
{
    m_callbacks[state] = callback;
}

void Transport::tick(float dt)
{
    if (m_state == State::Stopped)
    {
        return;
    }

    if (m_state == State::Looping)
    {
        m_playbackPosition = 0;
        setState(State::Starting);
    }

    if (m_state == State::Starting)
    {
        setState(State::Started);
    }

    if (m_state == State::Stopping)
    {
        setState(State::Stopped);
    }

    if (m_state == State::Started)
    {
        m_playbackPosition += dt;
        if (m_playbackPosition >= m_loopLength)
        {
            setState(State::Looping);
        }
    }
}

void Transport::callCallback(State state)
{
    auto it = m_callbacks.find(state);
    if (it != m_callbacks.end())
    {
        it->second(m_playbackPosition);
    }
}

void Transport::play()
{
    setState(State::Starting);
}

void Transport::stop()
{
    setState(State::Stopping);
}

const char *Transport::getStateName(Transport::State state)
{
    switch (state)
    {
    case Transport::State::Stopped:
        return "Stopped";
    case Transport::State::Stopping:
        return "Stopping";
    case Transport::State::Starting:
        return "Starting";
    case Transport::State::Started:
        return "Started";
    case Transport::State::Looping:
        return "Looping";
    }
}
