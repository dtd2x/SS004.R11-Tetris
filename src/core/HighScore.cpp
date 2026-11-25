#include "HighScore.h"

void HighScore::load(const std::string& p) { path = p; }
void HighScore::save() const {}
bool HighScore::add(const ScoreEntry&) { return false; }
std::vector<ScoreEntry> HighScore::top(Mode) const { return {}; }
