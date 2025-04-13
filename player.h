#ifndef PLAYER_H
#define PLAYER_H
#define WALK_FRAME_COUNT 12
#define IDLE_FRAME_COUNT 6
#define JUMP_FRAME_COUNT 4
#define FALL_FRAME_COUNT 4
#define FRAME_DELAY 70
#include <SDL2/SDL.h>
#include "bullet.h"
#include <vector>
using namespace std;
typedef enum {
    IDLE,
    WALK,
    JUMP,
    FALL
} PlayerAnimState;

typedef struct {
    float x, y, w, h;
    float vx,vy;
    float gravity;
    float jumpForce;

    int isMoving;
    int currentFrame;

    SDL_Texture* walkTexture;
    SDL_Texture* idleTexture;
    SDL_Texture* jumpTexture;
    SDL_Texture* fallTexture;
    SDL_Texture* bulletTexture;
    SDL_Rect bulletClips[4];
    vector <Bullet> bullets;

    Uint32 lastFrameTime;
    Uint32 frameResetTimer;
    Uint32 lastShotTime;

    SDL_RendererFlip flip;

    bool onGround;
    bool facingRight=true;

    PlayerAnimState currentAnim;
} Player;

void InitPlayer(Player* p);
void LoadPlayerTexture(Player* p, const char* walkPath,const char* idlePath, const char*jumpPath,const char* fallPath,const char* bulletPath);
void MovePlayer(Player* p, SDL_Event* event);
void UpdateMove(Player* p);
void RenderPlayer(Player* p);

#endif
