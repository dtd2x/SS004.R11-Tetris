#pragma once
#include "Theme.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Particles {
public:
    void burstRow(int row, const Theme& th);   // manh vun bay ra tu hang vua xoa
    void shake(float strength);                // rung man hinh (px), vd khi an Tetris
    void update(float dt);
    void draw(sf::RenderWindow& win) const;
    sf::Vector2f shakeOffset() const;          // Renderer cong vao vi tri board
    void clear();

private:
    struct P { sf::Vector2f pos, vel; sf::Color color; float life, maxLife; };
    std::vector<P> ps;
    float shakeTime = 0.f, shakeStrength = 0.f;
    sf::Vector2f offset;
};
