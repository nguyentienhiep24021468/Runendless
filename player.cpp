#include "player.h"
#include "window.h"
#include <SDL2/SDL_image.h>
#include <SDL.h>
void InitPlayer(Player* p) {
    p->x = 100;
    p->y = 400;
    p->w = 64;
    p->h = 64;
    p->vx=0;
    p->vy=0;
    p->walkTexture=0;
    p->idleTexture=0;
    p->currentFrame=0;
    p->lastFrameTime=0;
}

void LoadPlayerTexture(Player* p, const char* walkPath , const char* idlePath) {
    SDL_Surface* tempSurface = IMG_Load(walkPath);
    if (tempSurface) {
        p->walkTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
    }
    tempSurface= IMG_Load(idlePath);
    if (tempSurface) {
        p->idleTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
    }
}

void MovePlayer(Player* p, SDL_Event* event)
{
    if (event->type == SDL_KEYDOWN)
    {
        switch (event->key.keysym.sym) {
            case SDLK_a:
                p->vx=-4;
                p->isMoving=1;
                break;
            case SDLK_d:
                p->vx=+4;
                p->isMoving=1;
                break;
            case SDLK_w:
                p->vy=-4;
                p->isMoving=1;
                break;
            case SDLK_s:
                p->vy=+4;
                p->isMoving=1;
                break;
        }
    }
    if (event->type == SDL_KEYUP)
    {
        switch (event->key.keysym.sym) {
            case SDLK_a:
            case SDLK_d:
                p->vx=0;
                break;
            case SDLK_w:
            case SDLK_s:
                p->vy=0;
                break;
        }
    }
    if (p->vx ==0 && p->vy==0) p->isMoving=0;
}

void UpdateMove(Player* p) {
    p->x += p->vx;
    p->y += p->vy;

    if (p->x < 0) p->x = 0;
    if (p->x + p->w > 1280) p->x = 1280 - p->w;
    if (p->y < 0) p->y = 0;
    if (p->y + p->h > 640) p->y = 640 - p->h;
}

void RenderPlayer(Player* p) {
    Uint32 currentTime=SDL_GetTicks();
    SDL_Texture* currentTexture;
    int frameCount;

    if (p->isMoving)
    {
        currentTexture = p->walkTexture;
        frameCount = WALK_FRAME_COUNT;
        p->frameResetTimer = currentTime;

        if (currentTime > p->lastFrameTime + FRAME_DELAY)
        {
            p->currentFrame = (p->currentFrame + 1) % frameCount;
            p->lastFrameTime = currentTime;
        }
    }
    else
    {
        currentTexture = p->idleTexture;
        frameCount = IDLE_FRAME_COUNT;

        if (currentTime > p->frameResetTimer + 300) {
            if (currentTime > p->lastFrameTime + FRAME_DELAY) {
                p->currentFrame = (p->currentFrame + 1) % frameCount;
                p->lastFrameTime = currentTime;
            }
        }
        else p->currentFrame = 0;
    }

    SDL_Rect place = {p->x, p->y, p->w, p->h};
    SDL_Rect pictureframe= {p-> currentFrame*p->w,0,p->w,p->h};
    SDL_RenderCopy(renderer, currentTexture, &pictureframe, &place);
}
