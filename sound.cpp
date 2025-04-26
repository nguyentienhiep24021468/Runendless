#include "sound.h"
#include <iostream>

using namespace std;

bool SoundManager::LoadSounds() {
    bg_music = Mix_LoadMUS("music/background.mp3");
    gameover_music = Mix_LoadMUS("music/gameover.mp3");
    rungame_music=Mix_LoadMUS("music/runmusic.mp3");
    victory_music=Mix_LoadMUS("music/victory.mp3");
    if (!bg_music || !gameover_music || !rungame_music) {
        cout << "Lỗi khi load âm thanh: " << Mix_GetError() << endl;
        return false;
    }
    return true;
}

void SoundManager::StopMusic() {
    Mix_HaltMusic();
}

void SoundManager::PlayMusic(SoundType type) {
    Mix_Music* music = nullptr;
    if(type==GAMEOVER)  music = gameover_music;
    else music=victory_music;
    if (music) Mix_PlayMusic(music,0);
}

void SoundManager::PlayBackgroundMusic() {
    if (!Mix_PlayingMusic()) {
        Mix_PlayMusic(bg_music, -1);
    }
}

void SoundManager::PlayGameMusic() {
    if (!Mix_PlayingMusic()) {
        Mix_PlayMusic(rungame_music, -1);
    }
}

void SoundManager::FreeSounds() {
    Mix_FreeMusic(bg_music);
    Mix_FreeMusic(gameover_music);
    Mix_FreeMusic(rungame_music);
}
