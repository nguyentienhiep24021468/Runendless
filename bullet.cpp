#include "bullet.h"
#include <cmath>

void Bullet::update() {
    x += vx;
    distance_traveled += std::abs(vx);
    frame = (SDL_GetTicks() / 100) % 4;
    if (distance_traveled>200) active = false;
}

void Bullet::render(SDL_Renderer* renderer, SDL_Texture* bulletTexture, SDL_Rect bulletClips[]) {
    SDL_Rect dst = { (int)x, (int)y, 48, 48 };
    SDL_RenderCopy(renderer, bulletTexture, &bulletClips[frame], &dst);
}
