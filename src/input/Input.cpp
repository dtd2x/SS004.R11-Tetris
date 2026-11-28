#include "Input.h"
#include <SFML/Window/Keyboard.hpp>

static const float DAS = 0.17f;   // giu bao lau thi bat dau lap (giay)
static const float ARR = 0.05f;   // khoang cach giua 2 lan lap (giay)
static const Action REPEAT_ACTION[3] = { Action::Left, Action::Right, Action::SoftDrop };

static bool isHeld(int i) {
    using K = sf::Keyboard;
    switch (i) {
    case 0: return K::isKeyPressed(K::Left)  || K::isKeyPressed(K::A);
    case 1: return K::isKeyPressed(K::Right) || K::isKeyPressed(K::D);
    default: return K::isKeyPressed(K::Down) || K::isKeyPressed(K::S);
    }
}

void Input::handleEvent(const sf::Event& e) {
    if (e.type != sf::Event::KeyPressed) return;
    switch (e.key.code) {
    case sf::Keyboard::Left:  case sf::Keyboard::A: queue.push_back(Action::Left);     holdTime[0] = 0; break;
    case sf::Keyboard::Right: case sf::Keyboard::D: queue.push_back(Action::Right);    holdTime[1] = 0; break;
    case sf::Keyboard::Down:  case sf::Keyboard::S: queue.push_back(Action::SoftDrop); holdTime[2] = 0; break;
    case sf::Keyboard::Up:    case sf::Keyboard::W: queue.push_back(Action::Rotate);   break;
    case sf::Keyboard::Space:                       queue.push_back(Action::HardDrop); break;
    case sf::Keyboard::C:                           queue.push_back(Action::Hold);     break;
    case sf::Keyboard::Escape: case sf::Keyboard::P: queue.push_back(Action::Pause);   break;
    default: break;
    }
}

void Input::update(float dt) {
    for (int i = 0; i < 3; i++) {
        if (!isHeld(i)) { holdTime[i] = 0; repeatTimer[i] = 0; continue; }
        holdTime[i] += dt;
        if (holdTime[i] < DAS) continue;
        repeatTimer[i] += dt;
        while (repeatTimer[i] >= ARR) {
            repeatTimer[i] -= ARR;
            queue.push_back(REPEAT_ACTION[i]);
        }
    }
}

std::vector<Action> Input::takeActions() {
    std::vector<Action> out;
    out.swap(queue);
    return out;
}

void Input::reset() {
    queue.clear();
    for (int i = 0; i < 3; i++) { holdTime[i] = 0; repeatTimer[i] = 0; }
}
