#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
using namespace std;

class Menu {
public:
    Menu(SDL_Renderer* renderer);
    ~Menu();
    bool ShowStartMenu();
    int ShowGameOverMenu();

private:
    SDL_Renderer* gRenderer;
    SDL_Texture* startBackground;
    SDL_Texture* overBackground;


    SDL_Texture* LoadTexture(const string& path);
    bool IsInside(int x, int y, SDL_Rect rect);
};
