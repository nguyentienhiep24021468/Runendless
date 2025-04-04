#include <iostream>
#include "player.h"
#include "window.h"
#include <SDL.h>
#include <SDL_image.h>
using namespace std;

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (SDL_WaitEvent(&e))  // Chỉ thoát khi có sự kiện hợp lệ
    {
        if (e.key.keysym.sym == SDLK_SPACE) return;
        if (e.type == SDL_QUIT) return;
    }
}


SDL_Texture* LoadTexture(const char*filePath, SDL_Renderer* renderer)
{
    SDL_Texture* texture = nullptr;
    SDL_Surface* surface = IMG_Load(filePath);
    if (!surface)
    {
        cout <<"lỗi ảnh" << IMG_GetError()<< endl;
        return nullptr;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}


int SDL_main(int argc,  char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    if (!InitWindow())
    {
        cout << "error window :" << SDL_GetError()<< endl;
        return -1;
    }
    SDL_Texture* image= LoadTexture("picture/map1.png",renderer);
    if (!image)
    {
        SDL_Quit();
        return -1;
    }
    Player player;
    InitPlayer(&player);
    LoadPlayerTexture(&player,"picture/sheetwalk.png","picture/sheetidle.png");

    bool run=true;
    SDL_Event event;

    while(run)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) run=false;
            MovePlayer(&player,&event); // bàn phím
        }
        UpdateMove(&player);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,image ,NULL,NULL);
        RenderPlayer(&player); // player update giá trị
        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    waitUntilKeyPressed();
    SDL_DestroyTexture(image);
    quitSDL();
    IMG_Quit();
    return 0;
}
