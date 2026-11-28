
#include "Renderer.h"
#include "Layout.h"
#include "../Paths.h"
#include <cstdio>

bool Renderer::init() {
    return font.loadFromFile(assetPath("fonts/BeVietnamPro-SemiBold.ttf"));
}

void Renderer::drawText(sf::RenderWindow& win, const std::string& s, float x, float y,
                        unsigned size, sf::Color c, bool center) {
    sf::Text t(sf::String::fromUtf8(s.begin(), s.end()), font, size);
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

void Renderer::drawMini(sf::RenderWindow& win, int type, float x, float y, const Theme& th) {
    if (type == PIECE_NONE) return;

    Piece p;
    p.reset(type);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (p.shape[i][j] != ' ') {
                sf::RectangleShape r({ 22.f, 22.f });
                r.setPosition(x + j * 24.f, y + i * 24.f);
                r.setFillColor(th.piece[type]);
                win.draw(r);
            }
}

void Renderer::drawGame(sf::RenderWindow& win, Game& g, const Theme& th, sf::Vector2f off) {
    // Nen board
    sf::RectangleShape bg({ float(W * CELL), float(H * CELL) });
    bg.setPosition(BOARD_X + off.x, BOARD_Y + off.y);
    bg.setFillColor(th.boardBg);
    win.draw(bg);

    // Ghost
    int gy = g.ghostY();

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (g.cur.shape[i][j] != ' ' && gy != g.y)
                drawCell(win, cellX(g.x + j) + off.x,
                         cellY(gy + i) + off.y,
                         th.piece[g.cur.type], true);

    // Board
    for (int r = 0; r < H; r++)
        for (int c = 0; c < W; c++) {
            char ch = g.board[r][c];

            if (ch == CELL_EMPTY) continue;

            sf::Color col = (ch == CELL_WALL)
                ? th.wall
                : th.piece[pieceTypeOf(ch)];

            drawCell(win, cellX(c) + off.x, cellY(r) + off.y, col);
        }

    // Khung trai: HOLD
    drawText(win, "HOLD", 24, 20, 20, th.text);
    drawMini(win, g.holdType, 30, 60, th);

    // Khung phai: NEXT + chi so
    float rx = float(BOARD_X + W * CELL + 20);

    drawText(win, "NEXT", rx, 20, 20, th.text);

    for (int k = 0; k < 3; k++)
        drawMini(win, g.next[k], rx + 6, 60.f + k * 80.f, th);

    char buf[64];

    std::snprintf(buf, sizeof buf, "%d", g.score);
    drawText(win, "SCORE", rx, 330, 16, th.accent);
    drawText(win, buf, rx, 352, 24, th.text);

    std::snprintf(buf, sizeof buf, "%d", g.level);
    drawText(win, "LEVEL", rx, 400, 16, th.accent);
    drawText(win, buf, rx, 422, 24, th.text);

    std::snprintf(buf, sizeof buf, "%d", g.lines);
    drawText(win, "LINES", rx, 470, 16, th.accent);
    drawText(win, buf, rx, 492, 24, th.text);

    if (g.mode != Mode::Marathon) {
        float t = (g.mode == Mode::Ultra) ? 120.f - g.elapsed : g.elapsed;

        if (t < 0) t = 0;

        std::snprintf(buf, sizeof buf, "%d:%04.1f",
                      int(t) / 60, t - 60 * (int(t) / 60));

        drawText(win, "TIME", rx, 540, 16, th.accent);
        drawText(win, buf, rx, 562, 24, th.text);
    }
}

void Renderer::drawMenu(sf::RenderWindow&, const std::string&,
                        const std::vector<std::string>&, int, const Theme&) {}

void Renderer::drawResult(sf::RenderWindow&, const Game&, const HighScore&, const Theme&) {}
