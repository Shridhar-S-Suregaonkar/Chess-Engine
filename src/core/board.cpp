#include "board.h"
#include "header.h"
#include "moves.h"

#include "moves.cpp"

namespace board {

    Board::Board(const std::string& type){
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

    Board::Board(uint64_t wp, uint64_t wr, uint64_t wn, uint64_t wb, uint64_t wq, uint64_t wk) :
        whitePawns(wp), whiteRook(wr), whiteKnight(wn), whiteBishop(wb), whiteQueen(wq), whiteKing(wk),
        blackPawns(flipPiece(wp)), blackRook(flipPiece(wr)), blackKnight(flipPiece(wn)), blackBishop(flipPiece(wb)), blackQueen(flipPiece(wq)), blackKing(flipPiece(wk)) {}

    uint64_t Board::whitePieces() const {
        return whitePawns | whiteKnight | whiteBishop | whiteRook | whiteQueen | whiteKing;
    }

    uint64_t Board::blackPieces() const {
        return blackPawns | blackKnight | blackBishop | blackRook | blackQueen | blackKing;
    }

    uint64_t Board::allPieces() const {
        return whitePieces() | blackPieces();
    }

    void Board::printBoard() const {
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
                std::cout<<piece<<" ";

            }
            std::cout << "\n";
        }
        std::cout << "\n";

    }

    uint64_t Board::flipPiece(uint64_t b) const {
        b = ((b >> 8) & 0x00FF00FF00FF00FFULL) | ((b & 0x00FF00FF00FF00FFULL) << 8);
        b = ((b >> 16) & 0x0000FFFF0000FFFFULL) | ((b & 0x0000FFFF0000FFFFULL) << 16);
        b = (b >> 32) | (b << 32);
        return b;
    }
}


namespace chess {
    using namespace board;

    class chess{

        private:
            Board chessBoard;
            Color toMove;

    };
}