#pragma once

#include <deque>
#include <SDL.h>
#include "Apple.hpp"
#include "AssetsManager.hpp"

enum class Direction : uint8_t
{
    None,
    Up,
    Down,
    Left,
    Right,
};

class Snake
{
public:
    Snake();
    void Reset();
    void Move();
    void Grow();
    void Turn(Direction direction);
    void Draw(SDL_Renderer* renderer, const AssetsManager& assetsManager);
    bool IsSelfOrWallColliding() const;
    int PosX() const;
    int PosY() const;
    const std::deque<SDL_Rect>& Body() const;

private:
    std::deque<SDL_Rect> mBody;
    Direction mDirection;
    Direction mNextDirection;
    bool mGrowing;
};
