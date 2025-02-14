#pragma once

#include <SDL.h>

namespace Config
{
    inline const char* GameTitle = "Snake Game";
    inline const int ScreenWidth = 800;
    inline const int ScreenHeight = 480;
    inline const int GridSize = 40;
    inline const int ScreenX = SDL_WINDOWPOS_CENTERED;
    inline const int ScreenY = SDL_WINDOWPOS_CENTERED;
    inline const uint32_t WindowFlags = SDL_WINDOW_RESIZABLE;
    inline const uint32_t RendererFlags = SDL_RENDERER_ACCELERATED;
    inline const int FPS = 8;
    inline const int SnakeStarterSize = 3;
}
