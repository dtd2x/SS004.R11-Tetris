#include "Paths.h"
#include "core/Game.h"
#include "input/Input.h"
#include "render/Layout.h"
#include "render/Renderer.h"
#include "render/Theme.h"
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>

int main(int argc, char** argv) {
    (void)argc;
    initPaths(argv[0]);
    std::srand(unsigned(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(WIN_W, WIN_H), "Tetris - SS004.R11", sf::Style::Close);
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);   // giu phim do Input tu xu ly

    Game game;
    Input input;
    Renderer renderer;
    renderer.init();
    game.reset(Mode::Marathon);

    float fallTimer = 0.f;
    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;

        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) window.close();
            input.handleEvent(e);
        }

        if (!game.over && !game.won) {
            if (window.hasFocus()) input.update(dt);
            for (Action a : input.takeActions()) {
                switch (a) {
                case Action::Left:     game.moveLeft(); break;
                case Action::Right:    game.moveRight(); break;
                case Action::SoftDrop: if (game.softDrop()) fallTimer = 0; break;
		case Action::HardDrop: game.hardDrop(); fallTimer = 0; break;
                case Action::Rotate:   game.rotate(); break;
                default: break;
                }
            }
            game.update(dt);
            fallTimer += dt * 1000.f;
            if (fallTimer >= game.fallInterval()) { fallTimer = 0; game.tick(); }
            game.events.clear();
        }

        window.clear(THEMES[0].bg);
        renderer.drawGame(window, game, THEMES[0], { 0.f, 0.f });
        window.display();
    }
    return 0;
}
