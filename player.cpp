#include "player.h"
#include "window.h"
#include "map.h"
#include "bullet.h"
#include <SDL2/SDL_image.h>
#include <SDL.h>
#include <algorithm>
void InitPlayer(Player* p) {
    p->x = 128;
    p->y = 448;
    p->w = 64;
    p->h = 64;
    p->vx=0;
    p->vy=0;
    p->walkTexture=0;
    p->idleTexture=0;
    p->currentFrame=0;
    p->lastFrameTime=0;
    p->onGround=false;
    p->gravity = 0.5f;
    p->jumpForce= -15.0f;
    p->isMoving=0;
    p->lastShotTime=0;
}

void LoadPlayerTexture(Player* p, const char* walkPath , const char* idlePath, const char* jumpPath, const char* fallPath,const char* bulletPath) {
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
    tempSurface= IMG_Load(bulletPath);
    if (tempSurface)
    {
        p->bulletTexture=SDL_CreateTextureFromSurface(renderer, tempSurface);
        for (int i = 0; i < 4; i++) p->bulletClips[i] = { i * 64, 0, 64, 64 };
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
                p->flip = SDL_FLIP_HORIZONTAL; // left
                p->facingRight=false;
                break;
            case SDLK_d:
                p->vx=+4;
                p->isMoving=1;
                p->flip = SDL_FLIP_NONE; // right
                p->facingRight=true;
                break;
            case SDLK_k:
                if (p->onGround)
                {
                    p->vy=p->jumpForce;
                    p->onGround=false;
                }
                break;
            case SDLK_j:
                Uint32 currentTime=SDL_GetTicks();
                Bullet b;
                if (currentTime - p->lastShotTime >= 500)
                {
                    if (p->facingRight)
                    {
                        b.x = p->x +48;
                        b.y = p->y +16 ;
                        b.vx =13.0f;
                        p->bullets.push_back(b);
                    }
                    else
                    {
                        b.x = p->x -48;
                        b.y = p->y +16 ;
                        b.vx =-13.0f;
                        p->bullets.push_back(b);
                    }
                    p->lastShotTime= currentTime;
                }
                break;
        }
    }
    if (event->type == SDL_KEYUP)
    {
        switch (event->key.keysym.sym) {
            case SDLK_a:
            case SDLK_d:
                p->vx=0;
                p->isMoving=0;
                break;
        }
    }
}

void UpdateMove(Player* p) {
    p->vy+=p->gravity;
    if(p->vy>10) p->vy=10;


    float newX = p->x + p->vx;
    float newY = p->y + p->vy;

    bool hitX = false;
    for (int i = 0; i <= 1; i++) {
        int tileTop = (p->y + i * (p->h - 1)) / 64;
        int tileLeft = (newX) / 64;
        int tileRight = (newX + p->w - 1) / 64;

        if (Map[0][tileTop][tileLeft] == 1 || Map[0][tileTop][tileRight] == 1) {
            hitX = true;
            break;
        }
    }

    if (!hitX) p->x = newX;
    else p->vx = 0;

    bool hitY = false;
    p->onGround = false;
    for (int i = 0; i < 2; i++) {
        int checkX = (p->x + i * (p->w - 1)) / 64;
        int checkY = (newY + (p->vy > 0 ? p->h : 0)) / 64;

        if (Map[0][checkY][checkX] == 1) {
            hitY = true;

            if (p->vy > 0) {

                p->y = checkY * 64 - p->h;
                p->onGround = true;
            } else if (p->vy < 0) {

                p->y = (checkY + 1) * 64;
            }

            p->vy = 0;
            break;
        }
    }

    if (!hitY) p->y = newY;

    if (p->x < 0) p->x = 0;
    if (p->x + p->w > 1280) p->x = 1280 - p->w;
    if (p->y < 0) p->y = 0;
    if (p->y + p->h > 960)
    {
        p->y = 960 - p->h;
        p->vy=0;
        p->onGround=true;
    }

    for (auto& b : p->bullets) {
        if (b.active) b.update();
    }


    p->bullets.erase(
    remove_if(p->bullets.begin(), p->bullets.end(), [](Bullet& b) {
        return !b.active;
    }),
    p->bullets.end()
);


}

void RenderPlayer(Player* p) {
    Uint32 currentTime = SDL_GetTicks();
    SDL_Texture* currentTexture = nullptr;
    int frameCount = 1;
    PlayerAnimState newAnim;

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

    SDL_Rect place = {p->x, p->y, p->w, p->h};
    SDL_Rect pictureframe = {p->currentFrame * p->w, 0, p->w, p->h};
    SDL_RenderCopyEx(renderer, currentTexture, &pictureframe, &place, 0, NULL, p->flip);

    for (auto& b : p->bullets) {
        SDL_Rect src = p->bulletClips[b.frame];
        SDL_Rect dst = { (int)b.x, (int)b.y, 64, 64 };

        SDL_RenderCopy(renderer, p->bulletTexture, &src, &dst);
    }
}
