#pragma once
#include "../core/Game.h"

// Kich thuoc va vi tri tren man hinh (pixel). Ai can toa do o board thi dung file nay.
const int CELL    = 30;                 // canh 1 o
const int PANEL_W = 180;                // khung trai (hold) va khung phai (next, diem)
const int BOARD_X = PANEL_W;            // goc trai tren cua board
const int BOARD_Y = 0;
const int WIN_W   = PANEL_W + W * CELL + PANEL_W;   // 720
const int WIN_H   = H * CELL;                       // 630

// Toa do pixel goc trai tren cua o (hang r, cot c)
inline float cellX(int c) { return float(BOARD_X + c * CELL); }
inline float cellY(int r) { return float(BOARD_Y + r * CELL); }
