#include "player.h"
#include "window.h"
#include "map.h"
#include "bullet.h"
#include "bot.h"
#include "spike.h"
#include "sound.h"
#include <SDL.h>
#include <SDL_image.h>
#include <algorithm>
#include <vector>

using namespace std;

void InitPlayer(Player* p) {
    p->x = 144;
    p->y = 576;
    p->w = 64;
    p->h = 64;
    p->vx = 0;
    p->vy = 0;
    p->walkTexture = nullptr;
    p->idleTexture = nullptr;
    p->jumpTexture = nullptr;
    p->fallTexture = nullptr;
    p->bulletTexture = nullptr;
    p->dieTexture = nullptr;
    p->currentFrame = 0;
    p->lastFrameTime = 0;
    p->onGround = false;
    p->gravity = 0.5f;
    p->jumpForce = -16.0f;
    p->isMoving = 0;
    p->flip = SDL_FLIP_NONE;
    p->lastShotTime = 0;
    p->speedBoosting = false;
    p->speedBoostTime = 0;
    p->jumpBoostTime = 0;
    p->facingRight = true;
    p->isDead = false;
    p->dieFrame = 0;
    p->isWinning = false;
    p->isShooting = false;
    p->currentAnim = IDLE;
}

void LoadPlayerTexture(Player* p, const char* walkPath, const char* idlePath, const char* jumpPath, const char* fallPath, const char* bulletPath, const char* diePath) {
    SDL_Surface* tempSurface = IMG_Load(walkPath);
    if (tempSurface) {
        p->walkTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
    }
    tempSurface = IMG_Load(idlePath);
    if (tempSurface) {
        p->idleTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
    }
    tempSurface = IMG_Load(jumpPath);
    if (tempSurface) {
        p->jumpTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
    }
    tempSurface = IMG_Load(fallPath);
    if (tempSurface) {
        p->fallTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
    }
    tempSurface = IMG_Load(bulletPath);
    if (tempSurface) {
        p->bulletTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        for (int i = 0; i < 4; ++i) p->bulletClips[i] = { i * 64, 0, 64, 64 };
        SDL_FreeSurface(tempSurface);
    }
    tempSurface = IMG_Load(diePath);
    if (tempSurface) {
        p->dieTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
    }
}

void MovePlayer(Player* p, SDL_Event* event) {
    if (p->isDead) return;
    bool isMoving = false;

    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_a:
                p->vx = (p->speedBoosting ? -8.0f : -4.0f);
                isMoving = true;
                p->flip = SDL_FLIP_HORIZONTAL;
                p->facingRight = false;
                break;
            case SDLK_d:
                p->vx = (p->speedBoosting ? 8.0f : 4.0f);
                isMoving = true;
                p->flip = SDL_FLIP_NONE;
                p->facingRight = true;
                break;
            case SDLK_k:
                if (p->onGround) {
                    p->vy = p->jumpForce;
                    p->onGround = false;
                }
                break;
            case SDLK_j:
                Uint32 currentTime = SDL_GetTicks();
                if (currentTime - p->lastShotTime >= 500) {
                    Bullet b;
                    b.x = p->facingRight ? p->x + 48 : p->x - 48;
                    b.y = p->y + 16;
                    b.vx = p->facingRight ? 13.0f : -13.0f;
                    p->bullets.push_back(b);
                    p->isShooting = true;
                    p->lastShotTime = currentTime;
                }
                break;
        }
    }
    if (event->type == SDL_KEYUP) {
        switch (event->key.keysym.sym) {
            case SDLK_a:
            case SDLK_d:
                p->vx = 0;
                break;
        }
    }
    p->isMoving = (p->vx != 0);
}

void UpdateMove(Player* p) {
    const int TILE = 64;
    const int TOTAL_WIDTH = TILE * 180;
    const int TOTAL_HEIGHT = TILE * 15;

    if (p->isDead) {
        p->vx = 0;
        p->vy = 0;
        p->y = min(p->y, (float)TOTAL_HEIGHT - p->h);
        return;
    }

    if (SDL_GetTicks() - p->speedBoostTime > 5000) p->speedBoosting = false;
    if (SDL_GetTicks() - p->jumpBoostTime > 5000) p->jumpForce = -16.0f;

    p->vy += p->gravity;
    if (p->vy > 8) p->vy = 8;

    float newX = p->x + p->vx;
    float newY = p->y + p->vy;

    bool hitX = false;
    for (int i = 0; i <= 1; ++i) {
        int checkY = p->y + i * (p->h - 1);
        if (mapManager.CheckCollisionWithMap(newX, checkY, p->w, 1)) {
            hitX = true;
            break;
        }
    }
    if (!hitX) p->x = newX;

    bool hitY = false;
    p->onGround = false;
    for (int i = 0; i <= 1; ++i) {
        int checkX = p->x + i * (p->w - 1);
        int checkY = newY + (p->vy > 0 ? p->h : 0);
        if (mapManager.CheckCollisionWithMap(checkX, checkY, 1, 1)) {
            hitY = true;
            if (p->vy > 0) {
                p->y = ((checkY/TILE)-1)*TILE;
                p->onGround = true;
            } else {
                p->y = ((checkY / TILE)+ 1)*TILE;
            }
            p->vy = 0;
            break;
        }
    }
    if (!hitY) p->y = newY;

    if (p->y > 910) p->isDead = true;
    if (p->x >= TOTAL_WIDTH - p->w) p->isWinning = true;

    for (auto& b : p->bullets)
        if (b.active) b.update();

    p->bullets.erase(remove_if(p->bullets.begin(), p->bullets.end(), [](Bullet& b) { return !b.active; }),p->bullets.end());
}

void RenderPlayer(Player* p, int camX, int camY) {
    Uint32 currentTime = SDL_GetTicks();
    SDL_Texture* currentTexture = nullptr;
    int frameCount = 1;
    PlayerAnimState newAnim;

    if (p->isDead) {
        if (p->dieFrame < 24) {
            SDL_Rect clip = { p->dieFrame * 64, 0, 64, 64 };
            SDL_Rect renderQuad = { (int)p->x - camX, (int)p->y - camY, 64, 64 };
            SDL_RenderCopyEx(renderer, p->dieTexture, &clip, &renderQuad, 0, NULL, p->flip);
            if (currentTime > p->lastFrameTime + FRAME_DELAY) {
                p->dieFrame++;
                p->lastFrameTime = currentTime;
            }
        }
        return;
    }

    if (!p->onGround) {
        if (p->vy < 0) {
            currentTexture = p->jumpTexture;
            frameCount = JUMP_FRAME_COUNT;
            newAnim = JUMP;
        } else {
            currentTexture = p->fallTexture;
            frameCount = FALL_FRAME_COUNT;
            newAnim = FALL;
        }
    } else if (p->isMoving) {
        currentTexture = p->walkTexture;
        frameCount = WALK_FRAME_COUNT;
        p->frameResetTimer = currentTime;
        newAnim = WALK;
    } else {
        currentTexture = p->idleTexture;
        frameCount = IDLE_FRAME_COUNT;
        newAnim = IDLE;
    }

    if (newAnim != p->currentAnim) {
        p->currentAnim = newAnim;
        p->currentFrame = 0;
        p->lastFrameTime = currentTime;
    }

    if (currentTime > p->lastFrameTime + FRAME_DELAY) {
        p->currentFrame = (p->currentFrame + 1) % frameCount;
        p->lastFrameTime = currentTime;
    }

    SDL_Rect place = { (int)p->x - camX, (int)p->y - camY, p->w, p->h };
    SDL_Rect pictureframe = { p->currentFrame * p->w, 0, p->w, p->h };
    SDL_RenderCopyEx(renderer, currentTexture, &pictureframe, &place, 0, NULL, p->flip);

    for (auto& b : p->bullets) {
        SDL_Rect src = p->bulletClips[b.frame];
        SDL_Rect dst = { (int)b.x - camX, (int)b.y - camY, 64, 64 };
        SDL_RenderCopy(renderer, p->bulletTexture, &src, &dst);
    }
}

bool CheckCollisionWithBotAttack(const Player& player, const Bot& bot) {
    if (!bot.IsAttacking()) return false;

    SDL_Rect playerRect = { (int)player.x, (int)player.y, (int)player.w, (int)player.h };
    SDL_Rect botRect = { (int)bot.x, (int)bot.y, 64, 64 };

    return SDL_HasIntersection(&playerRect, &botRect);
}

bool CheckCollisionWithSpike(const Spike& spike, const Player& p) {
    SDL_Rect playerRect = { (int)p.x, (int)p.y, (int)p.w - 32, (int)p.h - 32 };
    SDL_Rect spikeRect = spike.GetRect();
    return SDL_HasIntersection(&playerRect, &spikeRect);
}

void HandlePlayerDeath(Player* p, const Spike& spike, const vector<Bot>& botList) {
    if (p->isDead) return;

    if (CheckCollisionWithSpike(spike, *p)) {
        p->isDead = true;
        p->dieFrame = 0;
        return;
    }

    for (const auto& bot : botList) {
        if (CheckCollisionWithBotAttack(*p, bot)) {
            p->isDead = true;
            p->dieFrame = 0;
            return;
        }
    }
}

bool IsDeathAnimDone(const Player* p) {
    return p->isDead && p->dieFrame >= 24;
}

void HandleBulletCollisionWithSpike(Player* p, Spike& spike) {
    for (auto& b : p->bullets) {
        if (!b.active) continue;

        SDL_Rect bulletRect = { (int)b.x + 64, (int)b.y, 64, 64 };
        SDL_Rect spikeRect = spike.GetRect();

        if (SDL_HasIntersection(&bulletRect, &spikeRect)) {
            b.active = false;
            spike.GetSlowed();
        }
    }
}

void Die(Player* p) {
    p->isDead = true;
}

void BoostSpeed(Player& p) {
    p.speedBoostTime = SDL_GetTicks();
    p.speedBoosting = true;
}

void BoostJump(Player& p) {
    p.jumpBoostTime = SDL_GetTicks();
    p.jumpForce = -21.0f;
}
