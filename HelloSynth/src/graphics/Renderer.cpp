#include "Renderer.h"

#include "utils/Logger.h"

int Renderer::init()
{
    log::info("Initializing Renderer...");

    SDL_DisplayMode displayMode;
    if (SDL_GetCurrentDisplayMode(0, &displayMode) == 0)
    {
        m_width = displayMode.w;
        m_height = displayMode.h;
    }

    // Setup the orintation hints
    SDL_SetHint("SDL_HINT_ORIENTATIONS", "LandscapeLeft LandscapeRight");

    m_window = SDL_CreateWindow("Hello, world!",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                m_width,
                                m_height,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN);
    if (!m_window)
    {
        log::error("Failed to create window: %s", SDL_GetError());
        return 1;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer)
    {
        log::error("Failed to create renderer: %s", SDL_GetError());
        return 1;
    }

    return 0;
}

Renderer::~Renderer()
{
    if (m_renderer)
    {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    if (m_window)
    {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

    SDL_Quit();
}