#pragma once
#include "Piece.h"
#include <vector>

const int H = 21;            // 20 hang choi (0..19) + 1 hang day (20)
const int W = 12;            // 10 cot choi (1..10) + 2 cot tuong (0, 11)
const char CELL_EMPTY = ' ';
const char CELL_WALL  = '#';

enum class Mode { Marathon, Sprint, Ultra };

// Game bao cho Audio / Particles biet chuyen gi vua xay ra.
// main.cpp doc het roi xoa (events.clear()) moi khung hinh.
enum class GameEvent { Move, Rotate, HardDrop, Lock, LineClear, Tetris, LevelUp, Hold, GameOver, Win };

struct Game {
    // ---------- du lieu ----------
    char  board[H][W];
    Piece cur;                       // khoi dang roi
    int   x = 0, y = 0;              // goc trai tren khung 4x4 cua khoi dang roi
    Bag   bag;
    int   next[3] = { PIECE_NONE, PIECE_NONE, PIECE_NONE };
    int   holdType = PIECE_NONE;
    bool  holdUsed = false;

    int   score = 0, level = 1, lines = 0;
    Mode  mode = Mode::Marathon;
    float elapsed = 0.f;             // so giay da choi
    bool  over = false;              // thua
    bool  won  = false;              // Sprint du 40 hang / Ultra het 120 giay

    std::vector<GameEvent> events;
    std::vector<int> clearedRows;    // cac hang vua xoa o lan khoa khoi gan nhat

    // ---------- ham cua thay (giu ten) ----------
    // QUY UOC: giua 2 thao tac, khoi dang roi LUON nam trong board.
    // Moi thao tac: boardDelBlock() -> kiem tra canMove / doi x,y -> block2Board().
    void initBoard();
    bool canMove(int dx, int dy);    // goi khi khoi da duoc xoa khoi board
    void block2Board();
    void boardDelBlock();
    int  removeLine();               // tra ve so hang da xoa, ghi vao clearedRows

    // ---------- ham them ----------
    void reset(Mode m);              // van moi
    void spawn();                    // lay khoi tiep theo; khong dat duoc thi over = true
    void moveLeft();
    void moveRight();
    bool softDrop();                 // roi 1 o; false neu da cham day
    void hardDrop();
    void rotate();                   // xoay co wall kick
    void hold();
    void tick();                     // het fallInterval: roi 1 o, cham day thi khoa
    void update(float dt);           // cong elapsed, kiem tra Ultra het gio
    int  fallInterval() const;       // ms giua 2 lan roi, theo level
    int  ghostY();                   // y thap nhat khoi hien tai roi toi duoc

private:
    void lockPiece();                // khoa khoi, xoa hang, cong diem, spawn
};
