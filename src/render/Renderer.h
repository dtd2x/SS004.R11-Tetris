#pragma once
#include "../core/Game.h"
#include "../core/HighScore.h"
#include "Theme.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Renderer {
public:
    bool init();   // nap font tu assets/fonts/; false neu thieu file

    // offset = do rung man hinh (Particles tinh), cong vao vi tri board
    void drawGame(sf::RenderWindow& win, Game& g, const Theme& th, sf::Vector2f offset);

    // Menu dung chung cho man hinh chinh, pause, game over: tieu de + danh sach muc, muc dang chon to sang
    void drawMenu(sf::RenderWindow& win, const std::string& title,
                  const std::vector<std::string>& items, int selected, const Theme& th);

    // Lop phu len tren board: game over / thang, kem bang top 10 cua mode vua choi
    void drawResult(sf::RenderWindow& win, const Game& g, const HighScore& hs, const Theme& th);

private:
    sf::Font font;
    void drawCell(sf::RenderWindow& win, float px, float py, sf::Color c, bool ghost = false);
    void drawText(sf::RenderWindow& win, const std::string& s, float x, float y,
                  unsigned size, sf::Color c, bool center = false);
    void drawMini(sf::RenderWindow& win, int type, float x, float y, const Theme& th);
};
