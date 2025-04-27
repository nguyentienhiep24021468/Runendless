#pragma once
#include "player.h"
#include <SDL.h>

class Spike {
public:
    void Init(SDL_Texture* texture, int screen_height);
    void Update(float deltaTime, float playerX, int screen_width);
    void Render(SDL_Renderer* renderer, int camera_x);

    float GetX() const { return x_pos; }
    SDL_Rect GetRect() const {
    return { (int)x_pos, 0, frameWidth, frameHeight };
    }

    void GetSlowed();
    bool is_slowed=false;
private:
    float x_pos;
    float speed = 150.0f;
    int frameWidth = 256;
    int frameHeight = 960;
    int currentFrame = 0;
    int frameCount = 4;
    float frameTime = 0.1f;
    float frameTimer = 0;

    SDL_Texture* texture = nullptr;
    SDL_Rect srcRect, destRect;
    float current_speed= 150.0f;
    Uint32 slow_timer;


};
extern Spike spike;
