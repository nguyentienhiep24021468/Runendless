#pragma once
#include <SDL.h>
#include <vector>

struct Bullet {
    float x, y, vx;
    float distance_traveled = 0;

    bool active = true;
    int frame = 0;

    void update();
    void render(SDL_Renderer* renderer, SDL_Texture* bulletTexture, SDL_Rect bulletClips[]);
};
