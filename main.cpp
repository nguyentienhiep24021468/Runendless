#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "player.h"
#include "window.h"
#include "map.h"
#include "bot.h"
#include "spike.h"
#include "menu.h"
#include "bullet.h"
#include "sound.h"
#include "item.h"

using namespace std;

int SDL_main(int argc, char* argv[])
{
    srand(time(nullptr));

    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    if (!InitWindow()) {
        cout << "error window: " << SDL_GetError() << endl;
        return -1;
    }

    SoundManager soundManager;

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        cout << "error sound: "<<SDL_GetError() << endl;
        exit(1);
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cout << "error sound: " << Mix_GetError() << endl;
        exit(1);
    }
    if (!soundManager.LoadSounds()) {
        cout << "error loadsound: " << endl;
        exit(1);
    }
    soundManager.PlayBackgroundMusic();

    SDL_Texture* tileTexture = LoadTexture("picture/sheetmap.png", renderer);
    Player player;
    InitPlayer(&player);
    LoadPlayerTexture(&player, "picture/sheetwalk.png", "picture/sheetidle.png", "picture/sheetjump.png", "picture/sheetfall.png", "picture/sheetbulletbig.png", "picture/sheetdie.png");
    LoadItemTextures(renderer);
    MapManager map;
    map.InitTestMap();
    if (!map.LoadTileset(renderer, "picture/sheetmap.png")) {
        cout << "Không load được tileset!" << endl;
        return -1;
    }

    Bullet bullet;
    InitAllBots(renderer);
    Spike spike;
    SDL_Texture* spikeTex = IMG_LoadTexture(renderer, "picture/sheetlog.png");
    Menu menu(renderer);

    if (!menu.ShowStartMenu()) return 0;
    soundManager.StopMusic();

    bool run = true, started = false, soundPlayed = false;
    SDL_Event event;

    int checkmoveforspike = 0;
    while (run)
    {
        Uint32 lastTick;
        if (!started) {
            spike.Init(spikeTex, 960);
            started = true;
            lastTick = SDL_GetTicks();
        }

        Uint32 currentTick = SDL_GetTicks();
        float deltaTime = (currentTick - lastTick) / 1000.0f;
        lastTick = currentTick;

        bool isRunning = true;
        if (isRunning && !soundPlayed) {
            soundManager.PlayGameMusic();
            soundPlayed = true;
        }

        while (SDL_PollEvent(&event) && isRunning)
        {
            if (event.type == SDL_QUIT) run = false;
            MovePlayer(&player, &event);
        }

        UpdateMove(&player);

        HandlePlayerDeath(&player, spike, botList);
        HandleItemCollection(player);
        HandleBulletCollisionWithSpike(&player, spike);
        if ((player.isDead && IsDeathAnimDone(&player)) || player.isWinning)
        {
            ClearItemsOnPlayerDeath(player);
            soundManager.StopMusic();
            if (player.isDead)  soundManager.PlayMusic(GAMEOVER);
            if (player.isWinning)
            {
                soundManager.PlayMusic(VICTORY);
                player.isWinning = false;
            }

            checkmoveforspike = 0;
            started = false;
            isRunning = false;
            soundPlayed = false;
            int result = menu.ShowGameOverMenu();

            if (result == 1 || result == 2) {
                if (result == 1) isRunning = true;

                soundManager.StopMusic();
                InitPlayer(&player);
                LoadPlayerTexture(&player, "picture/sheetwalk.png", "picture/sheetidle.png", "picture/sheetjump.png", "picture/sheetfall.png", "picture/sheetbulletbig.png", "picture/sheetdie.png");
                LoadItemTextures(renderer);
                spike.Init(spikeTex, 960);
                botList.clear();
                InitAllBots(renderer);
                map.camX = 0;
                map.camY = 0;
                player.bullets.clear();
                lastTick = SDL_GetTicks();

                if (result == 2) {
                    soundManager.PlayBackgroundMusic();
                    if (!menu.ShowStartMenu())    run = false;
                    soundManager.StopMusic();
                }

                continue;
            }
        }

        map.UpdateCamera(player.x, player.y, 1280, 960);

        if (player.x != 144 || player.y != 576) checkmoveforspike++;

        if (checkmoveforspike > 0)  spike.Update(deltaTime, player.x, 1064);

        SDL_RenderClear(renderer);
        map.Draw(renderer, 1280, 960);
        spike.Render(renderer, map.camX);

        for (auto& bot : botList)
        {
            bot.CheckBotDie(spike, player.bullets);
            bot.Update(player.x, player.y);
            bot.Render(renderer, map.camX, map.camY);
        }

        for (auto& item : itemList)   item.Render(renderer, map.camX);

        RenderPlayer(&player, map.camX, map.camY);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    waitUntilKeyPressed();
    SDL_DestroyTexture(tileTexture);
    SDL_DestroyTexture(spikeTex);

    for (auto& bot : botList) {
        SDL_DestroyTexture(bot.walkTexture);
        SDL_DestroyTexture(bot.attackTexture);
        SDL_DestroyTexture(bot.dieTexture);
    }

    soundManager.FreeSounds();
    quitSDL();
    IMG_Quit();
    return 0;
}
