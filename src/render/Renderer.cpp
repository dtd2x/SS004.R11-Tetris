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

void Renderer::drawMini(sf::RenderWindow& win, int type, float x, float y, const Theme& th) {
    if (type == PIECE_NONE) return;
    Piece p; p.reset(type);
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

    // khung trai: hold
    drawText(win, "HOLD", 24, 20, 20, th.text);
    drawMini(win, g.holdType, 30, 60, th);

    // khung phai: next + chi so
    float rx = float(BOARD_X + W * CELL + 20);
    drawText(win, "NEXT", rx, 20, 20, th.text);
    for (int k = 0; k < 3; k++) drawMini(win, g.next[k], rx + 6, 60.f + k * 80.f, th);

    char buf[64];
    std::snprintf(buf, sizeof buf, "%d", g.score);
    drawText(win, "SCORE", rx, 330, 16, th.accent);  drawText(win, buf, rx, 352, 24, th.text);
    std::snprintf(buf, sizeof buf, "%d", g.level);
    drawText(win, "LEVEL", rx, 400, 16, th.accent);  drawText(win, buf, rx, 422, 24, th.text);
    std::snprintf(buf, sizeof buf, "%d", g.lines);
    drawText(win, "LINES", rx, 470, 16, th.accent);  drawText(win, buf, rx, 492, 24, th.text);
    if (g.mode != Mode::Marathon) {
        float t = (g.mode == Mode::Ultra) ? 120.f - g.elapsed : g.elapsed;
        if (t < 0) t = 0;
        std::snprintf(buf, sizeof buf, "%d:%04.1f", int(t) / 60, t - 60 * (int(t) / 60));
        drawText(win, "TIME", rx, 540, 16, th.accent);  drawText(win, buf, rx, 562, 24, th.text);
    }
}

void Renderer::drawMenu(sf::RenderWindow& win, const std::string& title,
                        const std::vector<std::string>& items, int selected, const Theme& th) {
    sf::RectangleShape shade({ float(WIN_W), float(WIN_H) });
    shade.setFillColor(sf::Color(0, 0, 0, 170));
    win.draw(shade);
    drawText(win, title, WIN_W / 2.f, 120, 44, th.accent, true);
    for (size_t i = 0; i < items.size(); i++) {
        bool on = int(i) == selected;
        drawText(win, (on ? "> " : "") + items[i], WIN_W / 2.f, 240.f + i * 50.f, on ? 28 : 24,
                 on ? th.text : sf::Color(th.text.r, th.text.g, th.text.b, 150), true);
    }
}

void Renderer::drawResult(sf::RenderWindow& win, const Game& g, const HighScore& hs, const Theme& th) {
    sf::RectangleShape shade({ float(WIN_W), float(WIN_H) });
    shade.setFillColor(sf::Color(0, 0, 0, 190));
    win.draw(shade);
    drawText(win, g.won ? "HOÀN THÀNH!" : "GAME OVER", WIN_W / 2.f, 60, 40, th.accent, true);
    char buf[96];
    std::snprintf(buf, sizeof buf, "Điểm %d  ·  %d hàng  ·  %.1f giây", g.score, g.lines, g.elapsed);
    drawText(win, buf, WIN_W / 2.f, 120, 20, th.text, true);
    drawText(win, "TOP 10", WIN_W / 2.f, 170, 22, th.accent, true);
    auto top = hs.top(g.mode);
    for (size_t i = 0; i < top.size(); i++) {
        const ScoreEntry& e = top[i];
        if (g.mode == Mode::Sprint)
            std::snprintf(buf, sizeof buf, "%2zu. %6.1f s   %s", i + 1, e.seconds, e.date.c_str());
        else
            std::snprintf(buf, sizeof buf, "%2zu. %7d   %s", i + 1, e.score, e.date.c_str());
        drawText(win, buf, WIN_W / 2.f, 205.f + i * 28.f, 18, th.text, true);
    }
    drawText(win, "Enter: chơi lại    Esc: về menu", WIN_W / 2.f, 520, 18, th.text, true);
}
