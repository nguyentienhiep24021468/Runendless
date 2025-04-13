#include "map.h"
#include <iostream>
using namespace std;
int Map[MAP_COUNT][MAP_HEIGHT][MAP_WIDTH] =
{
    { //1
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0},
        {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
        {0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0},
        {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
        {0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0},
        {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0},
    }
};

int currentMap=0;
void LoadMap(int mapnow)
{
    if (mapnow>=0 && mapnow<MAP_COUNT) currentMap=mapnow;
}

void RenderMap(SDL_Renderer* renderer, SDL_Texture* tileTexture)
{
     for (int x = 0; x < MAP_HEIGHT; x++) {
        for (int y = 0; y < MAP_WIDTH; y++) {
            int tile=Map[currentMap][x][y];
            SDL_Rect rect= {y*TILE_SIZE,x*TILE_SIZE,TILE_SIZE,TILE_SIZE};
            SDL_RenderCopy(renderer, tileTexture, NULL, &rect);
        }
     }
}
