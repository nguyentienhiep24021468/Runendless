#pragma once

#define WALK_FRAME_COUNT 12
#define IDLE_FRAME_COUNT 6
#define JUMP_FRAME_COUNT 4
#define FALL_FRAME_COUNT 4
#define FRAME_DELAY 70


#include "bullet.h"
#include <vector>
#include <SDL2/SDL.h>

class Spike;
struct Bot;

typedef enum {
    IDLE,
    WALK,
    JUMP,
    FALL,
    SHOOT,
    DIE
} PlayerAnimState;

typedef struct {
    float x, y, w, h;
    float vx,vy;
    float gravity;
    float jumpForce;

    int isMoving;
    int currentFrame;
    int dieFrame;

    SDL_Texture* walkTexture;
    SDL_Texture* idleTexture;
    SDL_Texture* jumpTexture;
    SDL_Texture* fallTexture;
    SDL_Texture* bulletTexture;
    SDL_Texture* dieTexture;
    SDL_Rect bulletClips[4];
    std::vector <Bullet> bullets;

    Uint32 lastFrameTime;
    Uint32 frameResetTimer;
    Uint32 lastShotTime;

    SDL_RendererFlip flip;

    bool onGround;
    bool facingRight=true;
    bool isDead;
    bool isShooting=false;
    bool isJumping=false;
    bool isWinning=false;

    PlayerAnimState currentAnim;
} Player;

void InitPlayer(Player* p);
void LoadPlayerTexture(Player* p, const char* walkPath,const char* idlePath, const char*jumpPath,const char* fallPath,const char* bulletPath,const char* diePath);
void MovePlayer(Player* p, SDL_Event* event);
void UpdateMove(Player* p);
void RenderPlayer(Player* p, int camX, int camY);
void HandlePlayerDeath(Player* p, const Spike& spike, const std::vector<Bot>& botList);
bool CheckCollisionWithSpike(Spike& spike, const Player& player);
bool CheckCollisionWithBotAttack(const Player& player, const Bot& bot);
bool IsDeathAnimDone(const Player* p);
