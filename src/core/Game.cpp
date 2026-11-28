#include "Game.h"
#include <algorithm>

// ================= ham cua thay =================

void Game::initBoard() {
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            board[i][j] = (i == H - 1 || j == 0 || j == W - 1) ? CELL_WALL : CELL_EMPTY;
}

bool Game::canMove(int dx, int dy) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (cur.shape[i][j] != ' ') {
                int xt = x + j + dx;
                int yt = y + i + dy;
                if (xt < 1 || xt >= W - 1 || yt < 0 || yt >= H - 1) return false;
                if (board[yt][xt] != CELL_EMPTY) return false;
            }
    return true;
}

void Game::block2Board() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (cur.shape[i][j] != ' ') board[y + i][x + j] = cur.shape[i][j];
}

void Game::boardDelBlock() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (cur.shape[i][j] != ' ') board[y + i][x + j] = CELL_EMPTY;
}

int Game::removeLine() {
    clearedRows.clear();
    int removed = 0;
    for (int r = H - 2; r >= 0; r--) {
        bool full = true;
        for (int c = 1; c < W - 1; c++)
            if (board[r][c] == CELL_EMPTY) { full = false; break; }
        if (!full) continue;
        clearedRows.push_back(r - removed);   // vi tri hang truoc khi dich
        removed++;
        for (int k = r; k > 0; k--)
            for (int c = 1; c < W - 1; c++) board[k][c] = board[k - 1][c];
        for (int c = 1; c < W - 1; c++) board[0][c] = CELL_EMPTY;
        r++;                                  // kiem tra lai hang vua dich xuong
    }
    return removed;
}

// ================= ham them =================

void Game::reset(Mode m) {
    mode = m;
    score = 0; level = 1; lines = 0;
    elapsed = 0.f; over = false; won = false;
    holdType = PIECE_NONE; holdUsed = false;
    events.clear(); clearedRows.clear();
    bag = Bag();
    for (int i = 0; i < 3; i++) next[i] = bag.next();
    initBoard();
    spawn();
}

void Game::spawn() {
    cur.reset(next[0]);
    next[0] = next[1]; next[1] = next[2]; next[2] = bag.next();
    x = W / 2 - 2; y = 0;
    holdUsed = false;
    if (!canMove(0, 0)) {                 
        over = true;
        events.push_back(GameEvent::GameOver);
        return;
    }
    block2Board();
}
void Game::moveLeft() {
    boardDelBlock();
    bool ok = canMove(-1, 0);
    if (ok) x--;
    block2Board();
    if (ok) events.push_back(GameEvent::Move);
}

void Game::moveRight() {
    boardDelBlock();
    bool ok = canMove(1, 0);
    if (ok) x++;
    block2Board();
    if (ok) events.push_back(GameEvent::Move);
}

bool Game::softDrop() {
    boardDelBlock();
    bool ok = canMove(0, 1);
    if (ok) y++;
    block2Board();
    return ok;
}

void Game::hardDrop() {
}

void Game::rotate() {
    boardDelBlock();
    cur.rotateCW();
    bool ok = canMove(0, 0);
    if (!ok) cur.rotateCCW();
    block2Board();
    if (ok) events.push_back(GameEvent::Rotate);
}

void Game::hold() {
}

void Game::tick() {
    if (over || won) return;
    if (!softDrop()) lockPiece();
}

void Game::update(float dt) {
    if (over || won) return;
    elapsed += dt;
}

int Game::fallInterval() const {
    return 500;
}

int Game::ghostY() {
    return y;
}

void Game::lockPiece() {
    events.push_back(GameEvent::Lock);
    int n = removeLine();
    if (n > 0) {
        static const int POINTS[5] = { 0, 100, 300, 500, 800 };
        score += POINTS[n] * level;
        lines += n;
        events.push_back(n == 4 ? GameEvent::Tetris : GameEvent::LineClear);
        int newLevel = lines / 10 + 1;
        if (newLevel > level) {
            level = newLevel;
            events.push_back(GameEvent::LevelUp);
        }
    }
    spawn();
}
