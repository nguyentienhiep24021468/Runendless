#pragma once
#include <SDL2/SDL.h>
#include <vector>

struct Bullet {
    float x, y, vx;
    float distance_traveled = 0;

    bool active = true;
    int frame = 0;

    void update();
    void render(SDL_Renderer* renderer, SDL_Texture* bulletTexture, SDL_Rect bulletClips[]);

    SDL_Rect getRect() const {
    return SDL_Rect{ (int)x, (int)y, 48, 48 };
    }
};
