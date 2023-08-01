#include "WoozyWobbler.h"

#include "utils/Logger.h"

#include <SDL2/SDL.h>

WoozyWobbler::WoozyWobbler(){};

int WoozyWobbler::init()
{
    log::info("Initializing WoozyWobbler...");

    if (initSdl() != 0 || initAudio() != 0 || initVideo() != 0)
    {
        return 1;
    }

    return 0;
}

int WoozyWobbler::initSdl()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        log::error("Failed to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    return 0;
}

int WoozyWobbler::initAudio()
{
    m_audioSystem = new AudioSystem();

    // clang-format off
    m_audioSystem->setCallback(
        [](void *userData, uint8_t *stream, int len) 
        {
            auto self = (WoozyWobbler *)userData;
            auto config = self->m_audioSystem->getConfig();
            auto transport = self->m_transport;

            auto numChannels = config->channels;
            auto numSamples = len / sizeof(float);
            
            auto buffer = (float *)stream;
            for (size_t i = 0; i < numSamples; i += numChannels)
            {
                float x = ((float) rand() / (RAND_MAX)) * 2 - 1;
                for (size_t j = 0; j < numChannels; j++)
                {
                    buffer[i + j] = x * self->volume;
                }  
                transport->tick((float)numChannels / 48000.0f);
            }
        }
    );
    // clang-format on
    m_audioSystem->setUserData(this);

    if (m_audioSystem->init() != 0)
    {
        return 1;
    }

    return 0;
}

int WoozyWobbler::initVideo()
{
    m_renderer = new Renderer();
    if (m_renderer->init() != 0)
    {
        return 1;
    }

    return 0;
}

void WoozyWobbler::run()
{
    if (m_audioSystem->start() != 0)
    {
        return;
    }

    m_transport->setLoopLength(5.0f);
    m_transport->play();

    log::info("Running WoozyWobbler...");
    m_running = true;
    while (m_running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                m_running = false;
            }

            if (event.type == SDL_APP_WILLENTERBACKGROUND)
            {
                m_audioSystem->stop();
            }

            if (event.type == SDL_APP_WILLENTERFOREGROUND)
            {
                m_audioSystem->start();
            }
        }

        // Get the last touch position
        int x, y;
        SDL_GetMouseState(&x, &y);
        volume = (float)x / (float)m_renderer->getWidth();

        auto playbackPosition = m_transport->getPlaybackPosition() / m_transport->getLoopLength() * m_renderer->getWidth();

        SDL_SetRenderDrawColor(m_renderer->getInner(), 0, 255, y, 255);
        SDL_RenderClear(m_renderer->getInner());

        // Draw a 10px wide line to represent the playback position
        SDL_SetRenderDrawColor(m_renderer->getInner(), 255, 255, 255, 255);
        SDL_Rect rect = {
            (int)playbackPosition - 5,
            0,
            10,
            m_renderer->getHeight(),
        };
        SDL_RenderFillRect(m_renderer->getInner(), &rect);

        SDL_RenderPresent(m_renderer->getInner());
    }

    m_audioSystem->stop();
}

WoozyWobbler::~WoozyWobbler()
{
    delete m_transport;
    delete m_audioSystem;
    delete m_renderer;
}

int main(int argc, char *arv[])
{
    WoozyWobbler woozyWobbler;
    if (woozyWobbler.init() != 0)
    {
        return 1;
    }
    woozyWobbler.run();

    return 0;
}
