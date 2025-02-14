#pragma once

#include <array>
#include "Config.hpp"
#include "Snake.hpp"
#include "Apple.hpp"
#include "AssetsManager.hpp"

class Game
{
public:
    Game() : mRunning(true), mPaused(false), mScore(0),
             mWindow(nullptr), mRenderer(nullptr) {}

    void Init();
    void Run();
    void Exit();
    void RenderScene();
    void CleanScene();
    void HandleEvents();
    void HandleCollisions();
    void MoveSnake();
    void SpawnApples();
    void DrawObjects();
    void DrawScore();
    void DrawBackground();
    bool AppleHasValidPosition(Apple apple);
    int RandomInRange(int min, int max);

private:
    bool mRunning;
    bool mPaused;
    int mScore;
    AssetsManager mAssetsManager;
    Snake mSnake;
    std::array<Apple, 3> mApples;
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
};
