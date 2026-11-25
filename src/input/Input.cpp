#include "Input.h"

void Input::handleEvent(const sf::Event&) {}
void Input::update(float) {}
std::vector<Action> Input::takeActions() { std::vector<Action> out; out.swap(queue); return out; }
void Input::reset() { queue.clear(); }
