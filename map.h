#ifndef MAP_H
#define MAP_H
#include <SDL2/SDL.h>

#define MAP_COUNT 1
#define MAP_WIDTH 20
#define MAP_HEIGHT 15
#define TILE_SIZE 64

extern int Map[MAP_COUNT][MAP_HEIGHT][MAP_WIDTH];
extern int currentMap;

void LoadMap(int mapnow);
void RenderMap(SDL_Renderer* renderer, SDL_Texture* tileTexture);



#endif
