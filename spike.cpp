#include "spike.h"
#include "map.h"

void Spike::Init(SDL_Texture* tex, int screen_height) {
    texture = tex;
    x_pos = -frameWidth;
    frameHeight = screen_height;

    srcRect = { 0, 0, frameWidth, frameHeight };
    destRect = { (int)x_pos, 0, frameWidth, frameHeight };
}

void Spike::Update(float deltaTime, float playerX, int screen_width) {
    float targetSpeed = speed;

    if (x_pos < playerX - screen_width + frameWidth + 50) {
        targetSpeed = speed * 2.5f;
    }

    x_pos += targetSpeed * deltaTime;

    frameTimer += deltaTime;
    if (frameTimer >= frameTime) {
        frameTimer = 0;
        currentFrame = (currentFrame + 1) % frameCount;
        srcRect.x = currentFrame * frameWidth;
    }
    if (x_pos >= TOTAL_MAP_WIDTH * TILE_SIZE - 8 * TILE_SIZE)   x_pos = TOTAL_MAP_WIDTH * TILE_SIZE - 8 * TILE_SIZE;
}
void Spike::Render(SDL_Renderer* renderer, int camera_x) {
    destRect.x = (int)(x_pos - camera_x);
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}
