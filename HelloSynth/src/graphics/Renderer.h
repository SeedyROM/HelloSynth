#pragma once

#include <SDL2/SDL.h>

class Renderer
{
public:
    ~Renderer();

    int init();

    SDL_Renderer *getInner() const
    {
        return m_renderer;
    }

    int getWidth() const
    {
        return m_width;
    }

    int getHeight() const
    {
        return m_height;
    }

private:
    SDL_Window *m_window = nullptr;
    SDL_Renderer *m_renderer = nullptr;
    int m_width = 0;
    int m_height = 0;
};