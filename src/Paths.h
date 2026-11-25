#pragma once
#include <string>
// Goi 1 lan dau main(): initPaths(argv[0]);
void initPaths(const char* argv0);
// Duong dan toi file trong thu muc assets canh file exe, vd assetPath("sounds/move.wav")
std::string assetPath(const std::string& rel);
// Duong dan toi file du lieu canh file exe, vd dataPath("highscores.txt")
std::string dataPath(const std::string& file);