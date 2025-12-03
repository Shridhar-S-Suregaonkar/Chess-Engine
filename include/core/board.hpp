#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>


#include "header.hpp"
#include "moves.hpp"

namespace board {
    class BitBoard{
    protected:
        // White Pieces
        std::uint64_t whitePawns;
        std::uint64_t whiteKnight;
        std::uint64_t whiteBishop;
        std::uint64_t whiteRook;
        std::uint64_t whiteQueen;
        std::uint64_t whiteKing;
    
        // Black Pieces
        std::uint64_t blackPawns;
        std::uint64_t blackKnight;
        std::uint64_t blackBishop;
        std::uint64_t blackRook;
        std::uint64_t blackQueen;
        std::uint64_t blackKing;

    public:
        BitBoard() = default;
        BitBoard(std::uint64_t wp, std::uint64_t wr, std::uint64_t wn, std::uint64_t wb, std::uint64_t wq, std::uint64_t wk,
             std::uint64_t bp, std::uint64_t br, std::uint64_t bn, std::uint64_t bb, std::uint64_t bq, std::uint64_t bk)
        : whitePawns(wp), whiteRook(wr), whiteKnight(wn), whiteBishop(wb),
          whiteQueen(wq), whiteKing(wk),
          blackPawns(bp), blackRook(br), blackKnight(bn), blackBishop(bb),
          blackQueen(bq), blackKing(bk)
    {}
        
        void printBoard(Color player) const;
        std::uint64_t flipPiece(std::uint64_t b) const;
        bool checkBitBoard() const;
    };

    class Board: public BitBoard {
        
    private:
        
        bool _oo = true;
        bool _ooo = true;

    public:

        Board(const std::string& type = "Standard");
        Board(std::uint64_t wp, std::uint64_t wr, std::uint64_t wn, std::uint64_t wb, std::uint64_t wq, std::uint64_t wk);

        std::uint64_t whitePieces() const;
        std::uint64_t blackPieces() const;
        std::uint64_t allPieces() const;

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
        friend class ::eval::evaluator;
    };

}

namespace chess {
    class Chess{

        private:
            board::Board chessBoard;
            board::Color toMove;
            board::Color playerColor;

        public:
            Chess(const std::string& type = "Standard", board::Color player_color = board::Color::White, board::Color tm = board::Color::White): chessBoard(type), playerColor(player_color), toMove(tm){};
            void movePiece(const board::Move& m);
            void printBoard() const;
            bool isCheckmate() const;
            bool isStalemate() const;
            void play();


    };
}   

#endif //CHESS_BOARD_H