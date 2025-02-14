#include "Snake.hpp"
#include "Config.hpp"

Snake::Snake() : mDirection(Direction::None), mNextDirection(Direction::None), mGrowing(false)
{
    const int Grid = Config::GridSize;

    for (int i = 1; i <= Config::SnakeStarterSize; i++)
    {
        const int PosX = i * Grid;
        mBody.push_front({PosX, Grid, Grid, Grid});
    }
}

void Snake::Reset()
{
    mBody.clear();
    mDirection = Direction::None;
    mNextDirection = Direction::None;
    mGrowing = false;

    const int Grid = Config::GridSize;

    for (int i = 1; i <= Config::SnakeStarterSize; i++)
    {
        const int PosX = i * Grid;
        mBody.push_front({PosX, Grid, Grid, Grid});
    }
}

void Snake::Move()
{
    int posX = mBody.front().x;
    int posY = mBody.front().y;
    const int Grid = Config::GridSize;

    if (mNextDirection != Direction::None)
    {
        mDirection = mNextDirection;
        mNextDirection = Direction::None;
    }

    switch (mDirection)
    {
        case Direction::None:
            return;
        case Direction::Up:
            posY -= Grid;
            break;
        case Direction::Down:
            posY += Grid;
            break;
        case Direction::Left:
            posX -= Grid;
            break;
        case Direction::Right:
            posX += Grid;
            break;
    }

    mBody.push_front({posX, posY, Grid, Grid});

    if (!mGrowing)
        mBody.pop_back();
    else
        mGrowing = false;
}

void Snake::Grow()
{
    mGrowing = true;
}

void Snake::Turn(Direction direction)
{
    if ((mDirection == Direction::Up && direction == Direction::Down) ||
        (mDirection == Direction::Down && direction == Direction::Up) ||
        (mDirection == Direction::Left && direction == Direction::Right) ||
        (mDirection == Direction::Right && direction == Direction::Left))
        return;

    mNextDirection = direction;
}

void Snake::Draw(SDL_Renderer* renderer, const AssetsManager& assetsManager)
{
    SDL_Texture* curveTexture = assetsManager.GetTexture("SnakeCurve");
    SDL_Texture* headTexture = assetsManager.GetTexture("SnakeHead");
    SDL_Texture* segmentTexture = assetsManager.GetTexture("SnakeSegment");
    SDL_Texture* tailTexture = assetsManager.GetTexture("SnakeTail");

    SDL_Rect snakeHead = mBody.front();
    SDL_Rect snakeTail = mBody.back();

    double angle;

    switch (mDirection)
    {
        case Direction::Up:
            angle = -90;
            break;
        case Direction::Down:
            angle = 90;
            break;
        case Direction::Left:
            angle = 180;
            break;
        case Direction::Right:
            angle = 0;
            break;
    }

    SDL_RenderCopyEx(renderer, headTexture, nullptr, &snakeHead, angle, nullptr, SDL_FLIP_NONE);

    for (int i = 1; i < mBody.size() - 1; i++)
    {
        SDL_Rect segment = mBody[i];
        SDL_Rect nextSegment = mBody[i + 1];
        SDL_Rect lastSegment = mBody[i - 1];

        if ((nextSegment.x != lastSegment.x) && (nextSegment.y != lastSegment.y))
        {
            if ((nextSegment.x > segment.x && lastSegment.y < segment.y) ||
                (nextSegment.y < segment.y && lastSegment.x > segment.x))
                angle = 90;

            else if ((nextSegment.x > segment.x && lastSegment.y > segment.y) ||
                     (nextSegment.y > segment.y && lastSegment.x > segment.x))
                angle = 180;

            else if ((nextSegment.x < segment.x && lastSegment.y < segment.y) ||
                     (nextSegment.y < segment.y && lastSegment.x < segment.x))
                angle = 0;

            else if ((nextSegment.x < segment.x && lastSegment.y > segment.y) ||
                     (nextSegment.y > segment.y && lastSegment.x < segment.x))
                angle = -90;

            SDL_RenderCopyEx(renderer, curveTexture, nullptr, &segment, angle, nullptr, SDL_FLIP_NONE);
        }
        else
        {
            if (lastSegment.x < segment.x)
                angle = 0;

            else if (lastSegment.x > segment.x)
                angle = 180;

            else if (lastSegment.y < segment.y)
                angle = -90;

            else if (lastSegment.y > segment.y)
                angle = 90;

            SDL_RenderCopyEx(renderer, segmentTexture, nullptr, &segment, angle, nullptr, SDL_FLIP_NONE);
        }
    }

    SDL_Rect beforeTail = mBody[mBody.size() - 2];

    if (beforeTail.x < snakeTail.x)
        angle = 180;

    else if (beforeTail.x > snakeTail.x)
        angle = 0;

    else if (beforeTail.y < snakeTail.y)
        angle = -90;

    else if (beforeTail.y > snakeTail.y)
        angle = 90;

    SDL_RenderCopyEx(renderer, tailTexture, nullptr, &snakeTail, angle, nullptr, SDL_FLIP_NONE);
}

bool Snake::IsSelfOrWallColliding() const
{
    SDL_Rect snakeHead = mBody.front();

    const int Grid = Config::GridSize;
    const int LimitX = Config::ScreenWidth - Grid;
    const int LimitY = Config::ScreenHeight - Grid;

    if (snakeHead.x == LimitX || snakeHead.x == 0 || snakeHead.y == LimitY || snakeHead.y == 0)
        return true;

    for (int i = 1; i < mBody.size(); i++)
    {
        SDL_Rect segment = mBody[i];

        if (segment.x == snakeHead.x && segment.y == snakeHead.y)
            return true;
    }

    return false;
}

int Snake::PosX() const
{
    return mBody.front().x;
}

int Snake::PosY() const
{
    return mBody.front().y;
}

const std::deque<SDL_Rect>& Snake::Body() const
{
    return mBody;
}
