#include "Paths.h"
#include "core/Game.h"
#include "input/Input.h"
#include "render/Layout.h"
#include "render/Renderer.h"
#include "render/Theme.h"
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>

enum class Screen { Menu, Playing };

int main(int argc, char** argv) {
    (void)argc;
    initPaths(argv[0]);
    std::srand(unsigned(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(WIN_W, WIN_H), "Tetris - SS004.R11", sf::Style::Close);
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);   // tu xu ly giu phim trong Input (DAS/ARR)

    Game game;
    Input input;
    Renderer renderer;
    renderer.init();
    int themeIdx = loadThemeIndex();
    game.reset(Mode::Marathon);   // de man hinh menu co board phia sau
    game.events.clear();

    Screen screen = Screen::Menu;
    int menuSel = 0;
    float fallTimer = 0.f;
    sf::Clock clock;

    auto startGame = [&](Mode m) {
        game.reset(m);
        input.reset();
        fallTimer = 0.f;
        screen = Screen::Playing;
    };

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;
        const Theme& th = THEMES[themeIdx];

        // ---------- su kien ----------
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) window.close();
            if (screen == Screen::Playing) { input.handleEvent(e); continue; }
            if (e.type != sf::Event::KeyPressed) continue;
            auto k = e.key.code;

            if (screen == Screen::Menu) {
                const int N = 5;   // Marathon, Sprint, Ultra, Theme, Thoat
                if (k == sf::Keyboard::Up)   menuSel = (menuSel + N - 1) % N;
                if (k == sf::Keyboard::Down) menuSel = (menuSel + 1) % N;
                if (k == sf::Keyboard::Enter) {
                    if (menuSel == 0) startGame(Mode::Marathon);
                    if (menuSel == 1) startGame(Mode::Sprint);
                    if (menuSel == 2) startGame(Mode::Ultra);
                    if (menuSel == 3) { themeIdx = (themeIdx + 1) % THEME_COUNT; saveThemeIndex(themeIdx); }
                    if (menuSel == 4) window.close();
                }
            }
        }

        // ---------- cap nhat ----------
        if (screen == Screen::Playing) {
            if (window.hasFocus()) input.update(dt);
            for (Action a : input.takeActions()) {
                switch (a) {
                case Action::Left:     game.moveLeft(); break;
                case Action::Right:    game.moveRight(); break;
                case Action::SoftDrop: if (game.softDrop()) { game.score += 1; fallTimer = 0; } break;
                case Action::HardDrop: game.hardDrop(); fallTimer = 0; break;
                case Action::Rotate:   game.rotate(); break;
                case Action::Hold:     game.hold(); break;
                case Action::Pause:    screen = Screen::Menu; break;
                }
                if (game.over || game.won || screen != Screen::Playing) break;
            }

            if (screen == Screen::Playing && !game.over && !game.won) {
                game.update(dt);
                fallTimer += dt * 1000.f;
                if (fallTimer >= game.fallInterval()) { fallTimer = 0; game.tick(); }
            }

            game.events.clear();

            if (game.over || game.won) screen = Screen::Menu;
        }

        // ---------- ve ----------
        window.clear(th.bg);
        renderer.drawGame(window, game, th, { 0.f, 0.f });
        if (screen == Screen::Menu)
            renderer.drawMenu(window, "TETRIS",
                { "Marathon", "Sprint 40 hàng", "Ultra 2 phút", std::string("Theme: ") + th.name, "Thoát" },
                menuSel, th);
        window.display();
    }
    return 0;
}
