#include "item.h"
#include "player.h"
#include "map.h"
#include <SDL_image.h>
#include <cstdlib>  // Để sử dụng rand()
#include <iostream>

using namespace std;

MapManager map;
std::vector<Item> itemList;

static SDL_Texture* speedTexture = nullptr;
static SDL_Texture* jumpTexture = nullptr;
static SDL_Texture* deadTexture = nullptr;

Item::Item(float x, float y, ItemType type) {
    this->x_pos = x;
    this->y_pos = y;
    this->type = type;

    switch (type) {
        case ITEM_SPEED:
            texture = speedTexture;
            break;
        case ITEM_JUMP:
            texture = jumpTexture;
            break;
        case ITEM_DEAD:
            texture = deadTexture;
            break;
        default:
            texture = nullptr;
            break;
    }
}

void Item::Render(SDL_Renderer* renderer, int camera_x) {
    if (isCollected) return;

    SDL_Rect destRect = {(int)(x_pos - camera_x),(int)(y_pos),width,height};
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
}

SDL_Rect Item::GetRect() const {
    return { (int)x_pos, (int)y_pos, width, height };
}

bool Item::CheckCollision(const SDL_Rect& playerRect) {
    SDL_Rect itemRect = GetRect();
    return SDL_HasIntersection(&playerRect, &itemRect);
}

void SpawnItem(float x, float y) {
    int random = rand() % 3; // 0, 1, 2
    ItemType type = ITEM_NONE;

    if (random == 0) type = ITEM_SPEED;
    else if (random == 1) type = ITEM_JUMP;
    else type = ITEM_DEAD;

    itemList.push_back(Item(x, y, type));
}

void LoadItemTextures(SDL_Renderer* renderer) {
    speedTexture = IMG_LoadTexture(renderer, "picture/speed.png");
    jumpTexture = IMG_LoadTexture(renderer, "picture/jump.png");
    deadTexture = IMG_LoadTexture(renderer, "picture/dieitem.png");

    if (!speedTexture || !jumpTexture || !deadTexture) {
        cout << "Error loading item textures!" << endl;
    }
}

void FreeItemTextures() {
    if (speedTexture) SDL_DestroyTexture(speedTexture);
    if (jumpTexture) SDL_DestroyTexture(jumpTexture);
    if (deadTexture) SDL_DestroyTexture(deadTexture);
}


void HandleItemCollection(Player& player) {
    SDL_Rect playerRect = player.GetRect();

    for (auto& item : itemList) {
        if (!item.isCollected && item.CheckCollision(playerRect)) {
            item.isCollected = true;

            switch (item.GetType()) {
                case ITEM_SPEED:
                    BoostSpeed(player);
                    break;
                case ITEM_JUMP:
                    BoostJump(player);
                    break;
                case ITEM_DEAD:
                    player.isDead = true;
                    break;
                default:
                    break;
            }
        }
    }
}


void GameLoop(Player& player, SDL_Renderer* renderer, float deltaTime) {
    HandleItemCollection(player);
    HandleItemCollection(player);
    for (auto& item : itemList) {
        item.Render(renderer, map.camX);
    }
}

void ClearItemsOnPlayerDeath(const Player& player) {
    if (player.isDead|| player.isWinning) {
        itemList.clear();
    }
}
