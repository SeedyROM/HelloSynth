#pragma once

#include <SDL2/SDL.h>

class AudioSystem
{
public:
    ~AudioSystem();

    int init();
    int start();
    void stop();
    void setCallback(void (*callback)(void *, uint8_t *, int));

    template <typename T>
    void setUserData(T *userData)
    {
        m_userData = (void *)userData;
    }

    SDL_AudioSpec *getConfig() const;

private:
    SDL_AudioDeviceID m_deviceId = 0;
    SDL_AudioSpec *m_wantSpec = nullptr;
    SDL_AudioSpec *m_haveSpec = nullptr;

    void *m_userData = nullptr;
    void (*m_callback)(void *, uint8_t *, int) = nullptr;
};