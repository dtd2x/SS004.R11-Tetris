#pragma once
#include <SFML/Window/Event.hpp>
#include <vector>

enum class Action { Left, Right, SoftDrop, HardDrop, Rotate, Hold, Pause };

class Input {
public:
    void handleEvent(const sf::Event& e);   // bam 1 lan: xoay, hard drop, hold, pause, lan dau trai/phai/xuong
    void update(float dt);                  // giu phim trai/phai/xuong -> lap lai (DAS/ARR)
    std::vector<Action> takeActions();      // lay hang doi hanh dong roi xoa
    void reset();                           // goi khi bat dau van / tiep tuc sau pause

private:
    std::vector<Action> queue;
    float holdTime[3] = { 0, 0, 0 };        // thoi gian dang giu: 0 = trai, 1 = phai, 2 = xuong
    float repeatTimer[3] = { 0, 0, 0 };
};
