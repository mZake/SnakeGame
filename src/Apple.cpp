#include "Apple.hpp"
#include "Config.hpp"

void Apple::Reset()
{
    mPosX = -100;
    mPosY = -100;
    mVisible = false;
}

void Apple::Draw(SDL_Renderer* renderer, const AssetsManager& assetsManager)
{
    if (!mVisible)
        return;

    const int Grid = Config::GridSize;
    SDL_Texture* appleTexture = assetsManager.GetTexture("Apple");
    SDL_Rect rect {mPosX, mPosY, Grid, Grid};

    SDL_RenderCopy(renderer, appleTexture, nullptr, &rect);
}

void Apple::SetX(int pos)
{
    mPosX = pos;
}

void Apple::SetY(int pos)
{
    mPosY = pos;
}

void Apple::SetVisibility(bool visible)
{
    mVisible = visible;
}

int Apple::PosX() const
{
    return mPosX;
}

int Apple::PosY() const
{
    return mPosY;
}

bool Apple::IsVisible() const
{
    return mVisible;
}
