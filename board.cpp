#include <iostream>
#include <vector>

using namespace std;

class board{
private:
    // White Pieces
    uint64_t whitePawns;
    uint64_t whiteKnight;
    uint64_t whiteBishop;
    uint64_t whiteRook;
    uint64_t whiteQueen;
    uint64_t whiteKing;
    // Black Pieces
    uint64_t blackPawns;
    uint64_t blackKnight;
    uint64_t blackBishop;
    uint64_t blackRook;
    uint64_t blackQueen;
    uint64_t blackKing;


public:
    board(string type){
        if(type == "Standard"){
            whitePawns  = 0x000000000000FF00ULL;
            whiteRook   = 0x0000000000000081ULL;
            whiteKnight = 0x0000000000000042ULL;
            whiteBishop = 0x0000000000000024ULL;
            whiteQueen  = 0x0000000000000008ULL;
            whiteKing   = 0x0000000000000010ULL;

            blackPawns  = 0x00FF000000000000ULL;
            blackRook   = 0x8100000000000000ULL;
            blackKnight = 0x4200000000000000ULL;
            blackBishop = 0x2400000000000000ULL;
            blackQueen  = 0x0800000000000000ULL;
            blackKing   = 0x1000000000000000ULL;
        } else {
            whitePawns = whiteKnight = whiteBishop = whiteRook = whiteQueen = whiteKing = 0;
            blackPawns = blackKnight = blackBishop = blackRook = blackQueen = blackKing = 0;
        }

    }

    uint64_t whitePieces() const {
        return whitePawns | whiteKnight | whiteBishop | whiteRook | whiteQueen | whiteKing;
    }

    uint64_t blackPieces() const {
        return blackPawns | blackKnight | blackBishop | blackRook | blackQueen | blackKing;
    }

    uint64_t allPieces() const {
        return whitePieces() | blackPieces();
    }

    void printBoard() const {
        uint64_t pos;
        int p;
        char piece;
        for (int rank = 7; rank >= 0; rank--){
            for (int file = 0; file < 8; file++) {
                p = (rank << 3) + file;
                pos = 1ULL << p;
                piece = '.';
                if (whitePawns & pos) piece ='P';
                else if (whiteRook & pos) piece ='R';
                else if (whiteKnight & pos) piece ='N';
                else if (whiteBishop & pos) piece ='B';
                else if (whiteQueen & pos) piece ='Q';
                else if (whiteKing & pos) piece ='K';
                else if (blackPawns & pos) piece ='p';
                else if (blackRook & pos) piece ='r';
                else if (blackKnight & pos) piece ='n';
                else if (blackBishop & pos) piece ='b';
                else if (blackQueen & pos) piece ='q';
                else if (blackKing & pos) piece ='k';
                cout<<piece<<" ";

            }
            cout << "\n";
        }
        cout << "\n";

    }

    vector<pair<int, int>> whitePawnMoves() const {
        vector<pair<int, int>> moves;
        uint64_t empty = ~allPieces();
        uint64_t singlePush = (whitePawns << 8) & empty;
        uint64_t doublePush = ((singlePush & 0x000000000000FF00ULL) << 8) & empty;

        for (int sq = 0; sq < 64; sq++) {
            uint64_t pos = 1ULL << sq;
            if (singlePush & pos) {
                moves.emplace_back(sq - 8, sq);
            }
            if (doublePush & pos) {
                moves.emplace_back(sq - 16, sq);
            }
        }

        
    }
};
