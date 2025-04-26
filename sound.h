#pragma once

#include <SDL_mixer.h>
#include <string>
using namespace std;

enum SoundType {
    GAMEOVER,
    VICTORY,
    RUNGAME,
};

struct SoundManager {
    Mix_Music* bg_music = nullptr;
    Mix_Music* gameover_music = nullptr;
    Mix_Music* victory_music = nullptr;
    Mix_Music* rungame_music=nullptr;

    bool LoadSounds();
    void PlayMusic(SoundType type);
    void StopMusic();
    void FreeSounds();
    void PlayBackgroundMusic();
    void PlayGameMusic();
};
extern SoundManager soundManager;
