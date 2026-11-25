#include "Theme.h"
#include "../Paths.h"

const Theme THEMES[] = {
    { "Classic",
      sf::Color(20, 24, 38), sf::Color(10, 12, 20), sf::Color(35, 40, 58), sf::Color(90, 96, 120),
      sf::Color(235, 238, 245), sf::Color(80, 200, 220),
      { sf::Color(0, 240, 240), sf::Color(240, 240, 0), sf::Color(160, 0, 240), sf::Color(0, 240, 0),
        sf::Color(240, 0, 0), sf::Color(0, 0, 240), sf::Color(240, 160, 0) } },
};
const int THEME_COUNT = sizeof(THEMES) / sizeof(THEMES[0]);

int loadThemeIndex() {
    return 0;
}

void saveThemeIndex(int) {
}
