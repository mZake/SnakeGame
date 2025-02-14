#include <iostream>
#include <random>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Game.hpp"

void Game::Init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        mRunning = false;
        return;
    }
    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        std::cerr << "Failed to initialize SDL_image: " << SDL_GetError() << std::endl;
        mRunning = false;
        return;
    }
    if (TTF_Init() != 0)
    {
        std::cerr << "Failed to initialize SDL_ttf: " << SDL_GetError() << std::endl;
        mRunning = false;
        return;
    }

    mWindow = SDL_CreateWindow(Config::GameTitle, Config::ScreenX, Config::ScreenY,
                               Config::ScreenWidth, Config::ScreenHeight, Config::WindowFlags);

    if (!mWindow)
    {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        mRunning = false;
        return;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, Config::RendererFlags);

    if (!mRenderer)
    {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        mRunning = false;
        return;
    }

    mAssetsManager.LoadAssets(mRenderer, "assets");
}

void Game::Run()
{
    const int FrameDuration = 1000 / Config::FPS;
    uint64_t frameStart, frameTime;

    while (mRunning)
    {
        frameStart = SDL_GetTicks64();

        CleanScene();
        HandleEvents();
    
        if (!mPaused)
        {
            SpawnApples();
            MoveSnake();
            HandleCollisions();
        }

        DrawBackground();
        DrawObjects();
        DrawScore();
        RenderScene();

        frameTime = SDL_GetTicks64() - frameStart;

        if (frameTime < FrameDuration)
            SDL_Delay(FrameDuration - frameTime);
    }
}

void Game::Exit()
{
    SDL_DestroyWindow(mWindow);
    SDL_DestroyRenderer(mRenderer);
    mWindow = nullptr;
    mRenderer = nullptr;
    mAssetsManager.UnloadAssets();
    SDL_Quit();
}

void Game::RenderScene()
{
    SDL_RenderPresent(mRenderer);
}

void Game::CleanScene()
{
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0);
    SDL_RenderClear(mRenderer);
}

void Game::HandleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                mRunning = false;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                    SDL_RenderSetLogicalSize(mRenderer, Config::ScreenWidth, Config::ScreenHeight);
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_UP:
                        mSnake.Turn(Direction::Up);
                        break;
                    case SDLK_DOWN:
                        mSnake.Turn(Direction::Down);
                        break;
                    case SDLK_LEFT:
                        mSnake.Turn(Direction::Left);
                        break;
                    case SDLK_RIGHT:
                        mSnake.Turn(Direction::Right);
                        break;
                    case SDLK_p:
                        mPaused = !mPaused;
                        break;
                }
                break;
        }
    }
}

void Game::HandleCollisions()
{
    for (auto& apple : mApples)
    {
        if (apple.PosX() == mSnake.PosX() && apple.PosY() == mSnake.PosY())
        {
            mSnake.Grow();
            apple.Reset();
            mScore++;
            return;
        }
    }

    if (mSnake.IsSelfOrWallColliding())
    {
        for (auto& apple : mApples)
            apple.Reset();

        mSnake.Reset();
        mScore = 0;
    }
}

void Game::MoveSnake()
{
    mSnake.Move();
}

void Game::SpawnApples()
{
    const int Grid = Config::GridSize;

    for (auto& apple : mApples)
    {
        if (apple.IsVisible())
            continue;

        do
        {
            apple.SetX(RandomInRange(1, 18) * Grid);
            apple.SetY(RandomInRange(1, 10) * Grid);
        }
        while (!AppleHasValidPosition(apple));

        apple.SetVisibility(true);
    }
}

void Game::DrawObjects()
{
    mSnake.Draw(mRenderer, mAssetsManager);
    
    for (auto& apple : mApples)
        apple.Draw(mRenderer, mAssetsManager);
}

void Game::DrawScore()
{
    std::string text = "Score: " + std::to_string(mScore);
    TTF_Font* font = mAssetsManager.GetFont("OpenSans-SemiBold");
    SDL_Color color {255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surface);
    SDL_Rect rect {16, 2, surface->w, surface->h};

    SDL_RenderCopy(mRenderer, texture, nullptr, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Game::DrawBackground()
{
    SDL_Texture* texture0 = mAssetsManager.GetTexture("Background0");
    SDL_Texture* texture1 = mAssetsManager.GetTexture("Background1");
    SDL_Texture* texture2 = mAssetsManager.GetTexture("Background2");

    const int Grid = Config::GridSize;
    const int LimitX = Config::ScreenWidth - Grid;
    const int LimitY = Config::ScreenHeight - Grid;

    for (int y = 0; y < Config::ScreenHeight; y += Grid)
    {
        for (int x = 0; x < Config::ScreenWidth; x += Grid)
        {
            SDL_Rect rect {x, y, Grid, Grid};
            int modValue = Grid * 2;

            if (y == 0 || y == LimitY || x == 0 || x == LimitX)
                SDL_RenderCopy(mRenderer, texture2, nullptr, &rect);

            else if ((y % modValue == 0 && x % modValue == 0) || (y % modValue != 0 && x % modValue != 0))
                SDL_RenderCopy(mRenderer, texture0, nullptr, &rect);

            else
                SDL_RenderCopy(mRenderer, texture1, nullptr, &rect);
        }
    }
}

bool Game::AppleHasValidPosition(Apple apple)
{
    for (const auto& otherApple : mApples)
    {
        if (apple.PosX() == otherApple.PosX() && apple.PosY() == otherApple.PosY() && otherApple.IsVisible())
            return false;
    }

    for (const auto& segment : mSnake.Body())
    {
        if (apple.PosX() == segment.x && apple.PosY() == segment.y)
            return false;
    }

    return true;
}

int Game::RandomInRange(int min, int max)
{
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> distributor(min, max);

    return distributor(generator);
}
