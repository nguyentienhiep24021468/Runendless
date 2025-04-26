#pragma once

#define MAP_COUNT 9
#define MAP_WIDTH 20
#define MAP_HEIGHT 15
#define TILE_SIZE 64
#define TOTAL_MAP_WIDTH (MAP_WIDTH * MAP_COUNT)
#define TOTAL_MAP_HEIGHT MAP_HEIGHT
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <vector>

struct MapManager {
private:
    SDL_Texture* tileTexture = nullptr;

public:
    int camX = 0, camY = 0;

    bool LoadTileset(SDL_Renderer* renderer, const char* filePath);
    void InitTestMap();
    void UpdateCamera(int playerX, int playerY, int screenW, int screenH);
    void Draw(SDL_Renderer* renderer, int screenW, int screenH);

    bool CheckCollisionWithMap(int x, int y, int w, int h);

    SDL_Texture* GetTileTexture() const { return tileTexture; }
};

extern int Got[MAP_COUNT][MAP_HEIGHT][MAP_WIDTH];
extern int Map[15][180];
extern MapManager mapManager;
