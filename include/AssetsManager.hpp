#pragma once

#include <filesystem>
#include <unordered_map>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

class AssetsManager
{
public:
    void LoadAssets(SDL_Renderer* renderer, std::filesystem::path path);
    void UnloadAssets();
    SDL_Texture* GetTexture(std::string textureId) const;
    TTF_Font* GetFont(std::string fontId) const;

private:
    std::unordered_map<std::string, SDL_Texture*> mTextures;
    std::unordered_map<std::string, TTF_Font*> mFonts;
};
