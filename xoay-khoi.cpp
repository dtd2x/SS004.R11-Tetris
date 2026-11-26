#include "Piece.h"
#include <algorithm>

// Mang blocks cua thay, da sua: moi loai dung 1 lan, dung thu tu PieceType
static const char BLOCKS[PIECE_COUNT][4][4] = {
    {{' ',' ',' ',' '}, {'I','I','I','I'}, {' ',' ',' ',' '}, {' ',' ',' ',' '}},
    {{' ',' ',' ',' '}, {' ','O','O',' '}, {' ','O','O',' '}, {' ',' ',' ',' '}},
    {{' ',' ',' ',' '}, {' ','T',' ',' '}, {'T','T','T',' '}, {' ',' ',' ',' '}},
    {{' ',' ',' ',' '}, {' ','S','S',' '}, {'S','S',' ',' '}, {' ',' ',' ',' '}},
    {{' ',' ',' ',' '}, {'Z','Z',' ',' '}, {' ','Z','Z',' '}, {' ',' ',' ',' '}},
    {{' ',' ',' ',' '}, {'J',' ',' ',' '}, {'J','J','J',' '}, {' ',' ',' ',' '}},
    {{' ',' ',' ',' '}, {' ',' ','L',' '}, {'L','L','L',' '}, {' ',' ',' ',' '}},
};

int pieceTypeOf(char c) {
    for (int t = 0; t < PIECE_COUNT; t++)
        if (PIECE_CHAR[t] == c) return t;
    return PIECE_NONE;
}

void Piece::reset(int t) {
    type = t;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            shape[i][j] = BLOCKS[t][i][j];
}

void Piece::rotateCW() {
    if (type == PIECE_O) return;           // O xoay khong doi
    char old[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) old[i][j] = shape[i][j];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) shape[j][3 - i] = old[i][j];
}

void Piece::rotateCCW() {
    rotateCW(); rotateCW(); rotateCW();
}

Bag::Bag() : rng(std::random_device{}()) {}

int Bag::next() {
    return int(rng() % PIECE_COUNT);
}
