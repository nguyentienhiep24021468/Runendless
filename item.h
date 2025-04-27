#pragma once
#include "player.h"

#include <SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>

enum ItemType {
    ITEM_SPEED,
    ITEM_JUMP,
    ITEM_DEAD,
    ITEM_NONE
};

class Item {
public:
    Item(float x, float y, ItemType type);
    void Render(SDL_Renderer* renderer, int camera_x);
    SDL_Rect GetRect() const;
    bool CheckCollision(const SDL_Rect& playerRect);

    bool isCollected = false;
    ItemType GetType() const { return type; }

private:
    float x_pos, y_pos;
    ItemType type;
    int width = 32;
    int height = 32;
    SDL_Texture* texture = nullptr;
};

extern std::vector<Item> itemList;

void SpawnItem(float x, float y);
void LoadItemTextures(SDL_Renderer* renderer);
void FreeItemTextures();
void HandleItemCollection(Player& player);
void GameLoop(Player& player, SDL_Renderer* renderer, float deltaTime);
void ClearItemsOnPlayerDeath(const Player& player);
