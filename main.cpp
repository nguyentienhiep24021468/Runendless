
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>

#include "player.h"
#include "window.h"
#include "map.h"

using namespace std;


int SDL_main(int argc,  char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    if (!InitWindow())
    {
        cout << "error window :" << SDL_GetError()<< endl;
        return -1;
    }

    SDL_Texture* tileTexture = LoadTexture("picture/sheetmap.png", renderer);
    if (!tileTexture) {
        cout << "Không load được tile texture, thoát chương trình..." << endl;
        quitSDL();
        IMG_Quit();
        return -1;
    }

    Player player;
    InitPlayer(&player);
    LoadPlayerTexture(&player,"picture/sheetwalk.png","picture/sheetidle.png","picture/sheetjump.png","picture/sheetfall.png","picture/sheetbulletbig.png");

    MapManager map;
    map.InitTestMap();
    if (!map.LoadTileset(renderer, "picture/sheetmap.png")) {
        cout << "Không load được tileset!" << endl;
        return -1;
    }


    bool run = true;
    SDL_Event event;

    while (run)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) run = false;
            MovePlayer(&player, &event);
        }
        UpdateMove(&player);

        map.UpdateCamera((int)player.x, (int)player.y, 1280, 960);

        SDL_RenderClear(renderer);

        // 🎯 Vẽ map trước (camera đã cập nhật)
        map.Draw(renderer, 1280, 960);

        // 🎯 Vẽ player (render theo camera)
        RenderPlayer(&player, map.camX, map.camY);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    waitUntilKeyPressed();
    SDL_DestroyTexture(tileTexture);
    quitSDL();
    IMG_Quit();
    return 0;
}
