#pragma once

#include <SDL.h>
#include "AssetsManager.hpp"

class Apple
{
public:
    Apple() : mPosX(0), mPosY(0), mVisible(false) {}

    void Reset();
    void Draw(SDL_Renderer* renderer, const AssetsManager& textureManager);
    void SetX(int pos);
    void SetY(int pos);
    void SetVisibility(bool visible);
    int PosX() const;
    int PosY() const;
    bool IsVisible() const;

private:
    int mPosX;
    int mPosY;
    bool mVisible;
};
