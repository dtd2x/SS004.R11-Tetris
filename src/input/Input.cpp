#include "Input.h"

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

void Input::update(float) {
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
