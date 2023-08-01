#pragma once

#include <SDL2/SDL.h>

#include "audio/AudioSystem.h"
#include "audio/Transport.h"
#include "graphics/Renderer.h"

class WoozyWobbler
{
public:
    WoozyWobbler();
    ~WoozyWobbler();

    int initSdl();
    int initVideo();
    int initAudio();
    int init();

    void run();

    float volume = 0.0f;

private:
    Transport *m_transport;
    Renderer *m_renderer;
    AudioSystem *m_audioSystem;

    bool m_running = false;
};
