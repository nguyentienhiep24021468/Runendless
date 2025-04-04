#include "window.h"
#include <iostream>
using namespace std;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

int InitWindow() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        cout << "init error: " << SDL_GetError() << endl;
        return 0;
    }
    window = SDL_CreateWindow("Tower Fortress", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 640, SDL_WINDOW_SHOWN);
    if (!window)
    {
        cout << "create window error: " << SDL_GetError() << endl;
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        cout << "create renderer error: " << SDL_GetError() << endl;
        return 0;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"0"); // 0||1||2 ( độ nét tăng thì tốc độ giảm, 2 là nét nhất )
    SDL_RenderSetLogicalSize(renderer,1280, 640); // scale kích thước cho logic ( màn hình bao nhiêu cũng scale về cái trong ngoặc )
    return 1;
}

void quitSDL()
{
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}
