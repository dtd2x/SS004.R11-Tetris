#include "Renderer.h"
#include "Layout.h"
#include "../Paths.h"

bool Renderer::init() { return true; }
void Renderer::drawGame(sf::RenderWindow&, Game&, const Theme&, sf::Vector2f) {}
void Renderer::drawMenu(sf::RenderWindow&, const std::string&,
                        const std::vector<std::string>&, int, const Theme&) {}
void Renderer::drawResult(sf::RenderWindow&, const Game&, const HighScore&, const Theme&) {}
void Renderer::drawCell(sf::RenderWindow&, float, float, sf::Color, bool) {}
void Renderer::drawText(sf::RenderWindow&, const std::string&, float, float, unsigned, sf::Color, bool) {}
void Renderer::drawMini(sf::RenderWindow&, int, float, float, const Theme&) {}
