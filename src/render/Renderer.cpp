#include "Renderer.h"
#include "Layout.h"
#include "../Paths.h"
#include <cstdio>

bool Renderer::init() {
    return font.loadFromFile(assetPath("fonts/BeVietnamPro-SemiBold.ttf"));
}

void Renderer::drawText(sf::RenderWindow& win, const std::string& s, float x, float y,
                        unsigned size, sf::Color c, bool center) {
    sf::Text t(sf::String::fromUtf8(s.begin(), s.end()), font, size);   // fromUtf8 de hien tieng Viet
    t.setFillColor(c);
    if (center) {
        sf::FloatRect b = t.getLocalBounds();
        t.setOrigin(b.left + b.width / 2.f, 0.f);
    }
    t.setPosition(x, y);
    win.draw(t);
}

void Renderer::drawCell(sf::RenderWindow& win, float px, float py, sf::Color c, bool ghost) {
    sf::RectangleShape r({ CELL - 2.f, CELL - 2.f });
    r.setPosition(px + 1.f, py + 1.f);
    if (ghost) {
        r.setFillColor(sf::Color::Transparent);
        r.setOutlineThickness(-2.f);
        r.setOutlineColor(sf::Color(c.r, c.g, c.b, 140));
    } else {
        r.setFillColor(c);
    }
    win.draw(r);
}

void Renderer::drawMini(sf::RenderWindow&, int, float, float, const Theme&) {}

void Renderer::drawGame(sf::RenderWindow& win, Game& g, const Theme& th, sf::Vector2f off) {
    // nen board
    sf::RectangleShape bg({ float(W * CELL), float(H * CELL) });
    bg.setPosition(BOARD_X + off.x, BOARD_Y + off.y);
    bg.setFillColor(th.boardBg);
    win.draw(bg);

    // ghost: ve truoc de khoi that de len
    int gy = g.ghostY();
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (g.cur.shape[i][j] != ' ' && gy != g.y)
                drawCell(win, cellX(g.x + j) + off.x, cellY(gy + i) + off.y, th.piece[g.cur.type], true);

    // board (gom ca khoi dang roi, theo quy uoc cua thay)
    for (int r = 0; r < H; r++)
        for (int c = 0; c < W; c++) {
            char ch = g.board[r][c];
            if (ch == CELL_EMPTY) continue;
            sf::Color col = (ch == CELL_WALL) ? th.wall : th.piece[pieceTypeOf(ch)];
            drawCell(win, cellX(c) + off.x, cellY(r) + off.y, col);
        }
}

void Renderer::drawMenu(sf::RenderWindow&, const std::string&,
                        const std::vector<std::string>&, int, const Theme&) {}

void Renderer::drawResult(sf::RenderWindow&, const Game&, const HighScore&, const Theme&) {}
