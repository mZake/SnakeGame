#include <iostream>
#include <SDL_image.h>
#include "AssetsManager.hpp"

void AssetsManager::LoadAssets(SDL_Renderer* renderer, std::filesystem::path directory)
{
    std::filesystem::directory_iterator files(directory);

    for (const auto &file : files)
    {
        if (!file.is_regular_file())
            continue;

        std::string path = file.path().string();
        std::string assetId = file.path().stem().string();
        std::string extension = file.path().extension().string();

        if (extension == ".png")
        {
            SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
            if (!texture)
            {
                std::cerr << "Failed to load texture " << path << ": " << SDL_GetError() << std::endl;
                continue;
            }

            mTextures.emplace(assetId, texture);
        }

        if (extension == ".ttf")
        {
            TTF_Font* font = TTF_OpenFont(path.c_str(), 24);
            if (!font)
            {
                std::cerr << "Failed to load font " << path << ": " << SDL_GetError() << std::endl;
                continue;
            }

            mFonts.emplace(assetId, font);
        }
    }
}

void AssetsManager::UnloadAssets()
{
    for (const auto &texture : mTextures)
        SDL_DestroyTexture(texture.second);

    for (const auto& font : mFonts)
        TTF_CloseFont(font.second);

    mTextures.clear();
    mFonts.clear();
}

SDL_Texture* AssetsManager::GetTexture(const std::string& textureId) const
{
    auto texture = mTextures.find(textureId);
    if (texture == mTextures.end())
    {
        std::cerr << textureId << " is an invalid texture identifier!" << std::endl;
        return nullptr;
    }

    return texture->second;
}

TTF_Font* AssetsManager::GetFont(const std::string& fontId) const
{
    auto font = mFonts.find(fontId);
    if (font == mFonts.end())
    {
        std::cerr << fontId << " is an invalid font identifier!" << std::endl;
        return nullptr;
    }

    return font->second;
}
