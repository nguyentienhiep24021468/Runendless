#ifndef WINDOW_H
#define WINDOW_H
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <iostream>


extern SDL_Window* window;
extern SDL_Renderer* renderer;

int InitWindow();
void quitSDL();
void waitUntilKeyPressed();
SDL_Texture* LoadTexture(const char*filePath, SDL_Renderer* renderer);

#endif
