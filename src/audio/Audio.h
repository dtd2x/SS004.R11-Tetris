#pragma once
#include "../core/Game.h"
#include <SFML/Audio.hpp>
#include <deque>

class Audio {
public:
    bool load();                        // nap assets/sounds/*; thieu file van chay, chi khong co tieng
    void onEvent(GameEvent e);          // main goi cho moi su kien Game bao ra
    void playMenu();                    // tieng chon muc trong menu
    void playMusic();
    void pauseMusic();
    void stopMusic();
    void setMusicVolume(float v);       // 0..100
    void setSfxVolume(float v);         // 0..100

private:
    enum Sfx { MOVE, ROTATE, HARD_DROP, LOCK, LINE, TETRIS, LEVEL_UP, HOLD, GAME_OVER, WIN, MENU, SFX_COUNT };
    sf::SoundBuffer buf[SFX_COUNT];
    bool loaded[SFX_COUNT] = {};
    std::deque<sf::Sound> playing;      // sf::Sound phai song toi khi phat xong
    sf::Music music;
    bool hasMusic = false;
    float sfxVolume = 70.f;
    void play(Sfx s);
};
