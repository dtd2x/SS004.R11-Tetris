#pragma once
#include <random>
#include <vector>

// Thu tu co dinh: dung lam chi so mau trong Theme va ky tu trong board
enum PieceType { PIECE_I, PIECE_O, PIECE_T, PIECE_S, PIECE_Z, PIECE_J, PIECE_L, PIECE_COUNT };
const int PIECE_NONE = -1;

// Ky tu luu trong board cho tung loai khoi (giu cach lam cua thay)
const char PIECE_CHAR[PIECE_COUNT] = { 'I', 'O', 'T', 'S', 'Z', 'J', 'L' };

// Doi ky tu trong board ('I', 'O', ...) ve PieceType; tra PIECE_NONE neu khong phai khoi
int pieceTypeOf(char c);

struct Piece {
    int  type = PIECE_NONE;
    char shape[4][4] = {};   // ' ' hoac ky tu cua khoi

    void reset(int t);       // nap hinh goc cua loai t
    void rotateCW();         // xoay 90 do theo chieu kim dong ho trong khung 4x4
    void rotateCCW();        // xoay nguoc lai (dung de hoan tac)
};

// Bo sinh khoi 7-bag: moi luot 7 khoi ra du 7 loai, thu tu ngau nhien
class Bag {
public:
    Bag();
    int next();
private:
    std::vector<int> pool;
    std::mt19937 rng;
};