#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

extern SDL_Window* window;
extern SDL_Renderer* renderer;

int InitWindow();
void quitSDL();

#endif
