#pragma once
#include "Game.h"
#include <string>
#include <vector>

struct ScoreEntry {
    Mode        mode;
    int         score;
    int         lines;
    float       seconds;     // thoi gian choi (Sprint xep hang theo cot nay)
    std::string date;        // "2026-11-30"
};

class HighScore {
public:
    void load(const std::string& path);          // file khong co / hong -> danh sach rong, khong crash
    void save() const;
    bool add(const ScoreEntry& e);               // true neu lot vao top 10 cua mode do
    std::vector<ScoreEntry> top(Mode m) const;   // toi da 10, da sap xep
private:
    std::string path;
    std::vector<ScoreEntry> entries;
};
