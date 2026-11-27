#pragma once
#include "../core/Game.h"
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

struct OnlineEntry {
    std::string name;
    int         score = 0;
    int         lines = 0;
    float       seconds = 0.f;
    std::string date;
};

// Gui / doc bang xep hang online. Moi yeu cau chay tren luong rieng nen game khong bi dung;
// mat mang thi chi bao loi, van choi binh thuong.
class Online {
public:
    ~Online();
    void submit(const std::string& name, Mode m, int score, int lines, float seconds);
    void fetchTop(Mode m);

    bool busy() const;
    std::string status() const;                    // "", "Dang tai...", "Khong ket noi duoc", "Hang 3 online"
    std::vector<OnlineEntry> top(Mode m) const;

private:
    void run(std::function<void()> job);

    mutable std::mutex mtx;
    std::thread worker;
    bool working = false;
    std::string msg;
    std::vector<OnlineEntry> boards[3];
};
