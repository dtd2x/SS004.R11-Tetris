#pragma once
#include "../core/Piece.h"
#include <SFML/Graphics/Color.hpp>

struct Theme {
    const char* name;
    sf::Color bg, boardBg, grid, wall, text, accent;
    sf::Color piece[PIECE_COUNT];      // mau theo thu tu I O T S Z J L
};

extern const Theme THEMES[];
extern const int   THEME_COUNT;

// Luu / doc lua chon theme vao settings.txt canh file exe
int  loadThemeIndex();
void saveThemeIndex(int index);
