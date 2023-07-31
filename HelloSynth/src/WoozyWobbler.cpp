#include "WoozyWobbler.h"

#include "utils/Logger.h"

WoozyWobbler::WoozyWobbler(){};

int WoozyWobbler::init()
{
    log::info("Initializing WoozyWobbler...\n");

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
        log::error("Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    return 0;
}

int WoozyWobbler::initAudio()
{
    m_audioSystem = new AudioSystem();

    m_audioSystem->setCallback([](void *userData, uint8_t *stream, int len)
                               {

        float *buffer = (float *)stream;
        for(size_t i = 0; i < len / sizeof(float); i++)
        {
            float x = ((float) rand() / (RAND_MAX)) * 2 - 1;
            buffer[i] = x * 0.2f;
        } });

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
    m_audioSystem->start();

    log::info("Running WoozyWobbler...\n");
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
        }
        SDL_SetRenderDrawColor(m_renderer->getRenderer(), 0, 255, 0, 255);
        SDL_RenderClear(m_renderer->getRenderer());
        SDL_RenderPresent(m_renderer->getRenderer());
    }

    m_audioSystem->stop();
}

WoozyWobbler::~WoozyWobbler()
{
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
