#pragma once

#include "map.h"
#include "bullet.h"
#include <vector>
class Spike;

enum BotState {
    BOT_WALK,
    BOT_ATTACK,
    BOT_DIE
};

struct Bot {
    float x, y;
    float vx;

    bool facingLeft;
    bool isDead;
    bool spawnedItem;
    int state;
    int frame, frameTimer, frameDelay;

    int patrolLeftPx, patrolRightPx;

    SDL_Texture* walkTexture;
    SDL_Texture* attackTexture;
    SDL_Texture* dieTexture;

    void Init(int spawnX, int spawnY, SDL_Renderer* renderer);
    void Update(float playerX, float playerY);
    void Render(SDL_Renderer* renderer, int camX, int camY);
    bool IsAttacking() const {return state == BOT_ATTACK && !isDead;}
    void CheckBotDie(const Spike& spike,std::vector <Bullet>& bullets);
};

extern std::vector<Bot> botList;

void InitAllBots(SDL_Renderer* renderer);


