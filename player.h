#ifndef PLAYER_H
#define PLAYER_H
#define WALK_FRAME_COUNT 12
#define FRAME_DELAY 70
#define IDLE_FRAME_COUNT 6
#include <SDL2/SDL.h>

typedef struct {
    int x, y, w, h, vx, vy;
    SDL_Texture* walkTexture;
    SDL_Texture* idleTexture;
    int currentFrame;
    Uint32 lastFrameTime;
    int isMoving;
    Uint32 frameResetTimer;
} Player;

void InitPlayer(Player* p);
void LoadPlayerTexture(Player* p, const char* walkPath,const char* idlePath);
void MovePlayer(Player* p, SDL_Event* event);
void UpdateMove(Player* p);
void RenderPlayer(Player* p);

#endif
