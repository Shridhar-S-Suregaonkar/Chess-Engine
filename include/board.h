#include <iostream>
#include <vector>
#include <utility>

#include "header.h"

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

    board(uint64_t wp, uint64_t wr, uint64_t wn, uint64_t wb, uint64_t wq, uint64_t wk) :
          whitePawns(wp), whiteRook(wr), whiteKnight(wn), whiteBishop(wb), whiteQueen(wq), whiteKing(wk),
          blackPawns(flipPiece(wp)), blackRook(flipPiece(wr)), blackKnight(flipPiece(wn)), blackBishop(flipPiece(wb)), blackQueen(flipPiece(wq)), blackKing(flipPiece(wk)) {}

    board(uint64_t wp, uint64_t wr, uint64_t wn, uint64_t wb, uint64_t wq, uint64_t wk,
          uint64_t bp, uint64_t br, uint64_t bn, uint64_t bb, uint64_t bq, uint64_t bk) :
          whitePawns(wp), whiteRook(wr), whiteKnight(wn), whiteBishop(wb), whiteQueen(wq), whiteKing(wk),
          blackPawns(bp), blackRook(br), blackKnight(bn), blackBishop(bb), blackQueen(bq), blackKing(bk) {}

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

    uint64_t flipPiece(uint64_t b) const {
        b = ((b >> 8) & 0x00FF00FF00FF00FFULL) | ((b & 0x00FF00FF00FF00FFULL) << 8);
        b = ((b >> 16) & 0x0000FFFF0000FFFFULL) | ((b & 0x0000FFFF0000FFFFULL) << 16);
        b = (b >> 32) | (b << 32);
        return b;
    }

    vector<pair<uint8_t, uint8_t>> whitePawnQuietMoves() const {
        vector<pair<uint8_t, uint8_t>> moves;
        uint64_t occupied = allPieces();
        for (uint8_t sq = 0; sq <64; sq++){
            if (!(whitePawns & (1ULL<<sq))) continue;
            uint8_t rank = sq / 8;
            uint8_t file = sq % 8;

            uint8_t rto = rank + 1;
            uint8_t to = rto << 3 + file;
            if (rto < 8 && !(occupied & (1ULL << to)))
            {
                moves.emplace_back(sq, to);
                if (rank == 1)
                {
                    uint8_t rto2 = rank + 2;
                    uint8_t to2 = rto2 << 3 + file;
                    if (rto2 < 8 && !(occupied & (1ULL << to2)))
                    {
                        moves.emplace_back(sq, to2);
                    }
                }
            }
        }

        return moves;
    }

    vector<pair<uint8_t, uint8_t>> whitePawnCaptures() const {
        vector<pair<uint8_t, uint8_t>> moves;
        for (uint8_t sq = 0; sq < 64; sq++) {
            if (!(whitePawns & (1ULL << sq))) continue;
            uint8_t rank = sq / 8;
            uint8_t file = sq % 8;

            {
                uint8_t rto = rank + 1;
                int8_t fto = static_cast<int8_t>(file) - 1;
                if (rto < 8 && fto >= 0) {
                    uint8_t to = (rto << 3) + fto;
                    if (blackPieces() & (1ULL << to)) {
                        moves.emplace_back(sq, to);
                    }
                }
            }

            {
                uint8_t rto = rank + 1;
                int8_t fto = static_cast<int8_t>(file) + 1;
                if (rto < 8 && fto < 8) {
                    uint8_t to = (rto << 3) + fto;
                    if (blackPieces() & (1ULL << to)) {
                        moves.emplace_back(sq, to);
                    }
                }
            }
        }
        return moves;
    }

    vector<pair<uint8_t, uint8_t>> blackPawnQuietMoves() const {
        vector<pair<uint8_t, uint8_t>> moves;
        uint64_t occupied = allPieces();
        for (uint8_t sq = 0; sq < 64; sq++) {
            if (!(blackPawns & (1ULL << sq))) continue;
            uint8_t rank = sq / 8;
            uint8_t file = sq % 8;

            uint8_t rto = rank - 1;
            uint8_t to = rto << 3 + file;
            if (rto >= 0 && !(occupied & (1ULL << to)))
            {
                moves.emplace_back(sq, to);
                if (rank == 6)
                {
                    uint8_t rto2 = rank - 2;
                    uint8_t to2 = rto2 << 3 + file;
                    if (rto2 >= 0 && !(occupied & (1ULL << to2)))
                    {
                        moves.emplace_back(sq, to2);
                    }
                }
            }
        }

        return moves;
    }

    vector<pair<uint8_t, uint8_t>> blackPawnCaptures() const {
        vector<pair<uint8_t, uint8_t>> moves;
        for (uint8_t sq = 0; sq < 64; sq++) {
            if (!(blackPawns & (1ULL << sq))) continue;
            uint8_t rank = sq / 8;
            uint8_t file = sq % 8;

            {
                uint8_t rto = rank - 1;
                int8_t fto = static_cast<int8_t>(file) - 1;
                if ( rto >= 0 && fto >= 0) {
                    uint8_t to = (rto << 3) + fto;
                    if (whitePieces() & (1ULL << to)) {
                        moves.emplace_back(sq, to);
                    }
                }
            }

            {
                uint8_t rto = rank - 1;
                int8_t fto = static_cast<int8_t>(file) + 1;
                if (rto >= 0 && fto < 8) {
                    uint8_t to = (rto << 3) + fto;
                    if (whitePieces() & (1ULL << to)) {
                        moves.emplace_back(sq, to);
                    }
                }
            }
        }
        return moves;
    }

    vector<pair<uint8_t, uint8_t>> whiteKnightMoves() const {
        vector<pair<uint8_t, uint8_t>> moves;
        int8_t r_deltas[8] = {2, 2, 1, 1, -2, -2, -1, -1};
        int8_t f_deltas[8] = {1, -1, 2, -2, 1, -1, 2, -2};
        for (uint8_t sq = 0; sq < 64; sq++) {
            if (!(whiteKnight & (1ULL << sq))) continue;
            uint8_t rank = sq / 8;
            uint8_t file = sq % 8;
            for (uint8_t d = 0; d < 8; d++) {
                int8_t rto_i = static_cast<int8_t>(rank) + r_deltas[d];
                int8_t fto_i = static_cast<int8_t>(file) + f_deltas[d];
                if (rto_i >= 0 && rto_i < 8 && fto_i >= 0 && fto_i < 8) {
                    uint8_t rto = static_cast<uint8_t>(rto_i);
                    uint8_t fto = static_cast<uint8_t>(fto_i);
                    uint8_t to = rto * 8 + fto;
                    if (!(whitePieces() & (1ULL << to))) {
                        moves.push_back(make_pair(sq, to));
                    }
                }
            }
        }
        return moves;
    }

    vector<pair<uint8_t, uint8_t>> blackKnightMoves() const {
        vector<pair<uint8_t, uint8_t>> moves;
        int8_t r_deltas[8] = {2, 2, 1, 1, -2, -2, -1, -1};
        int8_t f_deltas[8] = {1, -1, 2, -2, 1, -1, 2, -2};
        for (uint8_t sq = 0; sq < 64; sq++) {
            if (!(blackKnight & (1ULL << sq))) continue;
            uint8_t rank = sq / 8;
            uint8_t file = sq % 8;
            for (uint8_t d = 0; d < 8; d++) {
                int8_t rto_i = static_cast<int8_t>(rank) + r_deltas[d];
                int8_t fto_i = static_cast<int8_t>(file) + f_deltas[d];
                if (rto_i >= 0 && rto_i < 8 && fto_i >= 0 && fto_i < 8) {
                    uint8_t rto = static_cast<uint8_t>(rto_i);
                    uint8_t fto = static_cast<uint8_t>(fto_i);
                    uint8_t to = rto * 8 + fto;
                    if (!(blackPieces() & (1ULL << to))) {
                        moves.push_back(make_pair(sq, to));
                    }
                }
            }
        }
        return moves;
    }

    vector<pair<uint8_t, uint8_t>> whiteBishopMoves() const {
        vector<pair<uint8_t, uint8_t>> moves;
        uint64_t occupied = allPieces();
        uint64_t opponent = blackPieces();
        for (uint8_t sq = 0; sq < 64; sq++) {
            if (!(whiteBishop & (1ULL << sq))) continue;
            uint8_t rank = sq / 8;
            uint8_t file = sq % 8;
            // Up-left
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) + static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file) - static_cast<int8_t>(dist);
                if (rto_i >= 8 || fto_i < 0) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Up-right
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) + static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file) + static_cast<int8_t>(dist);
                if (rto_i >= 8 || fto_i >= 8) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Down-left
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) - static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file) - static_cast<int8_t>(dist);
                if (rto_i < 0 || fto_i < 0) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Down-right
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) - static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file) + static_cast<int8_t>(dist);
                if (rto_i < 0 || fto_i >= 8) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
        }
        return moves;
    }

    vector<pair<uint8_t, uint8_t>> blackBishopMoves() const {
        vector<pair<uint8_t, uint8_t>> moves;
        uint64_t occupied = allPieces();
        uint64_t opponent = whitePieces();
        for (uint8_t sq = 0; sq < 64; sq++) {
            if (!(blackBishop & (1ULL << sq))) continue;
            uint8_t rank = sq / 8;
            uint8_t file = sq % 8;
            // Up-left
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) + static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file) - static_cast<int8_t>(dist);
                if (rto_i >= 8 || fto_i < 0) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Up-right
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) + static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file) + static_cast<int8_t>(dist);
                if (rto_i >= 8 || fto_i >= 8) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Down-left
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) - static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file) - static_cast<int8_t>(dist);
                if (rto_i < 0 || fto_i < 0) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Down-right
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) - static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file) + static_cast<int8_t>(dist);
                if (rto_i < 0 || fto_i >= 8) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
        }
        return moves;
    }

    vector<pair<uint8_t, uint8_t>> whiteRookMoves() const {
        vector<pair<uint8_t, uint8_t>> moves;
        uint64_t occupied = allPieces();
        uint64_t opponent = blackPieces();
        for (uint8_t sq = 0; sq < 64; sq++) {
            if (!(whiteRook & (1ULL << sq))) continue;
            uint8_t rank = sq / 8;
            uint8_t file = sq % 8;
            // Up
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) + static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file);
                if (rto_i >= 8) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Down
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) - static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file);
                if (rto_i < 0) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Right
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank);
                int8_t fto_i = static_cast<int8_t>(file) + static_cast<int8_t>(dist);
                if (fto_i >= 8) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Left
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank);
                int8_t fto_i = static_cast<int8_t>(file) - static_cast<int8_t>(dist);
                if (fto_i < 0) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
        }
        return moves;
    }

    vector<pair<uint8_t, uint8_t>> blackRookMoves() const {
        vector<pair<uint8_t, uint8_t>> moves;
        uint64_t occupied = allPieces();
        uint64_t opponent = whitePieces();
        for (uint8_t sq = 0; sq < 64; sq++) {
            if (!(blackRook & (1ULL << sq))) continue;
            uint8_t rank = sq / 8;
            uint8_t file = sq % 8;
            // Up
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) + static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file);
                if (rto_i >= 8) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Down
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) - static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file);
                if (rto_i < 0) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Right
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank);
                int8_t fto_i = static_cast<int8_t>(file) + static_cast<int8_t>(dist);
                if (fto_i >= 8) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Left
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank);
                int8_t fto_i = static_cast<int8_t>(file) - static_cast<int8_t>(dist);
                if (fto_i < 0) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
        }
        return moves;
    }

    vector<pair<uint8_t, uint8_t>> whiteQueenMoves() const {
        vector<pair<uint8_t, uint8_t>> moves;
        uint64_t occupied = allPieces();
        uint64_t opponent = blackPieces();
        for (uint8_t sq = 0; sq < 64; sq++) {
            if (!(whiteQueen & (1ULL << sq))) continue;
            uint8_t rank = sq / 8;
            uint8_t file = sq % 8;
            // Rook directions
            // Up
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) + static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file);
                if (rto_i >= 8) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Down
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) - static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file);
                if (rto_i < 0) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Right
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank);
                int8_t fto_i = static_cast<int8_t>(file) + static_cast<int8_t>(dist);
                if (fto_i >= 8) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Left
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank);
                int8_t fto_i = static_cast<int8_t>(file) - static_cast<int8_t>(dist);
                if (fto_i < 0) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Bishop directions
            // Up-left
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) + static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file) - static_cast<int8_t>(dist);
                if (rto_i >= 8 || fto_i < 0) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Up-right
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) + static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file) + static_cast<int8_t>(dist);
                if (rto_i >= 8 || fto_i >= 8) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Down-left
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) - static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file) - static_cast<int8_t>(dist);
                if (rto_i < 0 || fto_i < 0) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Down-right
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) - static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file) + static_cast<int8_t>(dist);
                if (rto_i < 0 || fto_i >= 8) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
        }
        return moves;
    }

    vector<pair<uint8_t, uint8_t>> blackQueenMoves() const {
        vector<pair<uint8_t, uint8_t>> moves;
        uint64_t occupied = allPieces();
        uint64_t opponent = whitePieces();
        for (uint8_t sq = 0; sq < 64; sq++) {
            if (!(blackQueen & (1ULL << sq))) continue;
            uint8_t rank = sq / 8;
            uint8_t file = sq % 8;
            // Rook directions
            // Up
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) + static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file);
                if (rto_i >= 8) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Down
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) - static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file);
                if (rto_i < 0) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Right
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank);
                int8_t fto_i = static_cast<int8_t>(file) + static_cast<int8_t>(dist);
                if (fto_i >= 8) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Left
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank);
                int8_t fto_i = static_cast<int8_t>(file) - static_cast<int8_t>(dist);
                if (fto_i < 0) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Bishop directions
            // Up-left
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) + static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file) - static_cast<int8_t>(dist);
                if (rto_i >= 8 || fto_i < 0) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Up-right
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) + static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file) + static_cast<int8_t>(dist);
                if (rto_i >= 8 || fto_i >= 8) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Down-left
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) - static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file) - static_cast<int8_t>(dist);
                if (rto_i < 0 || fto_i < 0) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
            // Down-right
            for (uint8_t dist = 1; ; dist++) {
                int8_t rto_i = static_cast<int8_t>(rank) - static_cast<int8_t>(dist);
                int8_t fto_i = static_cast<int8_t>(file) + static_cast<int8_t>(dist);
                if (rto_i < 0 || fto_i >= 8) break;
                uint8_t rto = static_cast<uint8_t>(rto_i);
                uint8_t fto = static_cast<uint8_t>(fto_i);
                uint8_t to = rto * 8 + fto;
                uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.push_back(make_pair(sq, to));
                    }
                    break;
                } else {
                    moves.push_back(make_pair(sq, to));
                }
            }
        }
        return moves;
    }

    vector<pair<uint8_t, uint8_t>> whiteKingMoves() const {
        vector<pair<uint8_t, uint8_t>> moves;
        int8_t r_deltas[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
        int8_t f_deltas[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
        for (uint8_t sq = 0; sq < 64; sq++) {
            if (!(whiteKing & (1ULL << sq))) continue;
            uint8_t rank = sq / 8;
            uint8_t file = sq % 8;
            for (uint8_t d = 0; d < 8; d++) {
                int8_t rto_i = static_cast<int8_t>(rank) + r_deltas[d];
                int8_t fto_i = static_cast<int8_t>(file) + f_deltas[d];
                if (rto_i >= 0 && rto_i < 8 && fto_i >= 0 && fto_i < 8) {
                    uint8_t rto = static_cast<uint8_t>(rto_i);
                    uint8_t fto = static_cast<uint8_t>(fto_i);
                    uint8_t to = rto * 8 + fto;
                    if (!(whitePieces() & (1ULL << to))) {
                        moves.push_back(make_pair(sq, to));
                    }
                }
            }
        }
        return moves;
    }

    vector<pair<uint8_t, uint8_t>> blackKingMoves() const {
        vector<pair<uint8_t, uint8_t>> moves;
        int8_t r_deltas[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
        int8_t f_deltas[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
        for (uint8_t sq = 0; sq < 64; sq++) {
            if (!(blackKing & (1ULL << sq))) continue;
            uint8_t rank = sq / 8;
            uint8_t file = sq % 8;
            for (uint8_t d = 0; d < 8; d++) {
                int8_t rto_i = static_cast<int8_t>(rank) + r_deltas[d];
                int8_t fto_i = static_cast<int8_t>(file) + f_deltas[d];
                if (rto_i >= 0 && rto_i < 8 && fto_i >= 0 && fto_i < 8) {
                    uint8_t rto = static_cast<uint8_t>(rto_i);
                    uint8_t fto = static_cast<uint8_t>(fto_i);
                    uint8_t to = rto * 8 + fto;
                    if (!(blackPieces() & (1ULL << to))) {
                        moves.push_back(make_pair(sq, to));
                    }
                }
            }
        }
        return moves;
    }

};