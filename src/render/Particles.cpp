#include "Particles.h"

void Particles::burstRow(int, const Theme&) {}
void Particles::shake(float) {}
void Particles::update(float) {}
void Particles::draw(sf::RenderWindow&) const {}
sf::Vector2f Particles::shakeOffset() const { return offset; }
void Particles::clear() {}
