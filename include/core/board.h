#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>


#include "header.h"
#include "pieces.h"

namespace board {

    class Board{

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
        Board(const std::string& type = "Standard");
        Board(uint64_t wp, uint64_t wr, uint64_t wn, uint64_t wb, uint64_t wq, uint64_t wk);

        uint64_t whitePieces() const;
        uint64_t blackPieces() const;
        uint64_t allPieces() const;
        void printBoard() const;
        uint64_t flipPiece(uint64_t b) const;

        std::vector<Move> whitePawnQuietMoves() const;
        std::vector<Move> whitePawnCaptures() const;
        std::vector<Move> blackPawnQuietMoves() const;
        std::vector<Move> blackPawnCaptures() const;
        std::vector<Move> blackKnightMoves() const;
        std::vector<Move> whiteKnightMoves() const;
        std::vector<Move> whiteBishopMoves() const;
        std::vector<Move> blackBishopMoves() const;
        std::vector<Move> whiteRookMoves() const;
        std::vector<Move> blackRookMoves() const;
        std::vector<Move> whiteQueenMoves() const;
        std::vector<Move> blackQueenMoves() const;
        std::vector<Move> whiteKingMoves() const;
        std::vector<Move> blackKingMoves() const;
    };

}

#endif //CHESS_BOARD_H