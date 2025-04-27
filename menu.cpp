#include "menu.h"
#include <iostream>
using namespace std;

Menu::Menu(SDL_Renderer* renderer) {
    gRenderer = renderer;
    startBackground = LoadTexture("picture/menu.png");
    if (!startBackground)   cout << "error menu.png";

    overBackground = LoadTexture("picture/menudie.png");
    if (!overBackground)    cout << "error menudie.png";
}

Menu::~Menu() {
    SDL_DestroyTexture(startBackground);
    SDL_DestroyTexture(overBackground);
}

SDL_Texture* Menu::LoadTexture(const string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        cout << "error load picture: "<< IMG_GetError() << endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);

    if (!texture)   cout << "error load picture: "<< SDL_GetError() << endl;

    return texture;
}

bool Menu::IsInside(int x, int y, SDL_Rect rect) {
    return (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h);
}

bool Menu::ShowStartMenu() {
    SDL_Event e;
    SDL_Rect playRect = {448, 640, 384, 128};

    while (true) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) exit(0);
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;
                if (IsInside(x, y, playRect)) return true;
            }
        }

        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, startBackground, NULL, NULL);
        SDL_RenderPresent(gRenderer);
        SDL_Delay(10);
    }
}

int Menu::ShowGameOverMenu() {
    SDL_Event e;
    SDL_Rect replayRect = {192, 576, 192, 192};
    SDL_Rect menuRect = {896, 576, 192, 192};

    while (true) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) exit(0);
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;
                if (IsInside(x, y, replayRect)) return 1;
                if (IsInside(x, y, menuRect)) return 2;
            }
        }

        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, overBackground, NULL, NULL);
        SDL_RenderPresent(gRenderer);
        SDL_Delay(10);
    }
}
