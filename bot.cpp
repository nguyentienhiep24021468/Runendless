#include "bot.h"
#include "map.h"
#include "spike.h"
#include "sound.h"
#include <cmath>
#include <vector>

using namespace std;

vector<Bot> botList;
SoundManager soundManager;

void Bot::Init(int spawnX, int spawnY, SDL_Renderer* renderer) {
    x = spawnX;
    y = spawnY;
    facingLeft = false;
    isDead = false;
    state = BOT_WALK;
    frame = 0;
    frameTimer = 0;
    frameDelay = 10;

    patrolLeftPx  = spawnX  - TILE_SIZE * 5;
    patrolRightPx = spawnX  + TILE_SIZE * 5;

    walkTexture   = IMG_LoadTexture(renderer, "picture/botwalk.png");
    attackTexture = IMG_LoadTexture(renderer, "picture/botattack.png");
    dieTexture    = IMG_LoadTexture(renderer, "picture/botdeath.png");
}

void Bot::Update(float playerX, float playerY) {
    if (isDead) {
        state = BOT_DIE;
        if (++frameTimer >= frameDelay) {
            frameTimer = 0;
            if (frame < 6) frame++;
        }
        return;
    }

    float dx = playerX - x;
    float dy = fabs(playerY - y);

    if (fabs(dx) < TILE_SIZE && dy < TILE_SIZE) {
        state = BOT_ATTACK;
    }

    if (state == BOT_ATTACK) {
        if (++frameTimer >= frameDelay) {
            frameTimer = 0;
            if (frame < 5) {
                frame++;
            } else {
                frame = 0;
            }
        }

        if (fabs(dx) > TILE_SIZE * 2 || dy > TILE_SIZE * 2) {
            state = BOT_WALK;
        }
        return;
    }

    state = state == BOT_ATTACK ? BOT_ATTACK : BOT_WALK;
    bool inChaseRange = fabs(dx) < TILE_SIZE * 6 && dy < TILE_SIZE * 2;

    float patrolSpeed = 1.5f;
    float chaseSpeed  = 2.5f;
    float speed;
    float nextX;

    float homeX = (patrolLeftPx + patrolRightPx) * 0.5f;

    if (inChaseRange) {
        facingLeft = dx < 0;
        speed = chaseSpeed;
        nextX = x + (facingLeft ? -speed : speed);

    } else if (x < patrolLeftPx || x > patrolRightPx) {
        facingLeft = x > homeX;
        speed = patrolSpeed;

        if (x < homeX) {
            nextX = x + speed;
        } else {
            nextX = x - speed;
        }

    } else {
        speed = patrolSpeed;
        nextX = x + (facingLeft ? -speed : speed);
    }

    bool blockAhead = mapManager.CheckCollisionWithMap(
        int(nextX), int(y), TILE_SIZE, TILE_SIZE
    );
    int footX = int(nextX + (facingLeft ? -TILE_SIZE / 2 : TILE_SIZE / 2));
    int footY = int(y + TILE_SIZE);
    bool noFloor = !mapManager.CheckCollisionWithMap(footX, footY, TILE_SIZE / 2, TILE_SIZE / 2);

    if (blockAhead || noFloor) {
        if (!inChaseRange && (x >= patrolLeftPx && x <= patrolRightPx)) {
            facingLeft = !facingLeft;
            nextX = x + (facingLeft ? -speed : speed);
        } else {
            nextX = x;
        }
    }

    if (!inChaseRange && (nextX < patrolLeftPx || nextX > patrolRightPx)) {
        nextX = (nextX < patrolLeftPx ? patrolLeftPx : patrolRightPx);
        facingLeft = (nextX == patrolRightPx);
    }

    x = nextX;

    if (state != BOT_ATTACK) {
        if (++frameTimer >= frameDelay) {
            frameTimer = 0;
            frame = (frame + 1) % 4;
        }
    }
}

void Bot::Render(SDL_Renderer* renderer, int camX, int camY) {
    SDL_Texture* tex = isDead
        ? dieTexture
        : (state == BOT_WALK ? walkTexture
           : state == BOT_ATTACK ? attackTexture
           : dieTexture);

    SDL_Rect src  = { frame * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE };
    SDL_Rect dst  = { int(x) - camX, int(y) - camY, TILE_SIZE, TILE_SIZE };
    SDL_RendererFlip flip = facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderCopyEx(renderer, tex, &src, &dst, 0, nullptr, flip);
}

void InitAllBots(SDL_Renderer* renderer) {
    vector<pair<int, int>> positions = {
        {640, 768}, {960, 512}, {1856, 512}, {2688, 448}, {3008, 704},
        {3648, 768}, {3648 + 8 * 64, 768 - 3 * 64}, {3648 + 12 * 64, 768},
        {3648 + 14 * 64, 768 - 5 * 64}, {3648 + 26 * 64, 768 - 5 * 64},
        {3648 + 28 * 64, 768}, {3648 + 46 * 64, 768}, {3648 + 53 * 64, 768},
        {3648 + 57 * 64, 768 - 5 * 64}, {3648 + 71 * 64, 768},
        {3648 + 76 * 64, 768 - 7 * 64}, {3648 + 89 * 64, 768 - 7 * 64},
        {3648 + 90 * 64, 768 - 3 * 64}, {3648 + 97 * 64, 768 - 1 * 64},
        {3648 + 105 * 64, 768}, {3648 + 109 * 64, 768}, {3648 + 113 * 64, 768}
    };
    for (auto [bx, by] : positions) {
        Bot b;
        b.Init(bx, by, renderer);
        botList.push_back(b);
    }
}

void Bot::CheckBotDie(const Spike& spike, vector<Bullet>& bullets) {
    if (isDead) return;
    if (spike.GetX() >= x - 150) {
        isDead = true;
        state = BOT_DIE;
        frame = 0;
        frameTimer = 0;
        return;
    }

    SDL_Rect botRect = { int(x), int(y), TILE_SIZE, TILE_SIZE };
    for (auto& b : bullets) {
        if (!b.active) continue;
        SDL_Rect bulletRect = { int(b.x), int(b.y), 32, 32 };
        SDL_Rect reducedBotRect = { int(x) + 10, int(y) + 10, TILE_SIZE - 30, TILE_SIZE - 30 };
        SDL_Rect reducedBulletRect = { int(b.x) + 5, int(b.y) + 5, 32 - 10, 32 - 10 };
        if (SDL_HasIntersection(&reducedBotRect, &reducedBulletRect)) {
            isDead = true;
            state = BOT_DIE;
            frame = 0;
            frameTimer = 0;
            b.active = false;
            break;
        }
    }
}
