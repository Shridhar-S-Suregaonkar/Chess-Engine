#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>


#include "header.hpp"
#include "moves.hpp"
#include "../eval/evaluator.hpp"

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

        bool _oo = true;
        bool _ooo = true;

    public:

        uint64_t* const whitePiece_iter[6] = {&whitePawns, &whiteKnight, &whiteBishop, &whiteRook, &whiteQueen, &whiteKing};
        uint64_t* const blackPiece_iter[6] = {&blackPawns, &blackKnight, &blackBishop, &blackRook, &blackQueen, &blackKing};

        Board(const std::string& type = "Standard");
        Board(uint64_t wp, uint64_t wr, uint64_t wn, uint64_t wb, uint64_t wq, uint64_t wk);

        uint64_t whitePieces() const;
        uint64_t blackPieces() const;
        uint64_t allPieces() const;
        void printBoard(Color player) const;
        uint64_t flipPiece(uint64_t b) const;
        bool checkBitBoard() const;

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

        friend class chess;
        friend std::vector<Move> movesEvaluator(const Board& board, Color toMove);
        friend class eval::evaluator;
    };

}

namespace chess {
    using namespace board;
    class Chess{

        private:
            Board chessBoard;
            Color toMove;
            Color playerColor;

        public:
            Chess(const std::string& type = "Standard", Color player_color = White, Color tm = White): chessBoard(type), playerColor(player_color), toMove(tm){};
            void movePiece(const Move& m);
            void printBoard() const;
            bool isCheckmate() const;
            bool isStalemate() const;
            void play();


    };
}   

#endif //CHESS_BOARD_H