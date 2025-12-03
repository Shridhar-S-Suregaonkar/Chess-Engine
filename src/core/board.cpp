#include <cstdint>
#include <iostream>

#include "board.hpp"
#include "header.hpp"
#include "moves.hpp"

// #include "moves.cpp"

namespace board {

    Board::Board(const std::string& type) {
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

    Board::BitBoard::BitBoard(std::uint64_t wp, std::uint64_t wr, std::uint64_t wn, std::uint64_t wb, std::uint64_t wq, std::uint64_t wk,
         std::uint64_t bp, std::uint64_t br, std::uint64_t bn, std::uint64_t bb, std::uint64_t bq, std::uint64_t bk)
        : whitePawns(wp), whiteRook(wr), whiteKnight(wn), whiteBishop(wb),
          whiteQueen(wq), whiteKing(wk),
          blackPawns(bp), blackRook(br), blackKnight(bn), blackBishop(bb),
          blackQueen(bq), blackKing(bk) {}

    Board::Board(std::uint64_t wp, std::uint64_t wr, std::uint64_t wn, std::uint64_t wb, std::uint64_t wq, std::uint64_t wk)
        : BitBoard(wp, wr, wn, wb, wq, wk,
        flipPiece(wp), flipPiece(wr), flipPiece(wn),
        flipPiece(wb), flipPiece(wq), flipPiece(wk)) {}


    std::uint64_t Board::whitePieces() const {
        return whitePawns | whiteKnight | whiteBishop | whiteRook | whiteQueen | whiteKing;
    }

    std::uint64_t Board::blackPieces() const {
        return blackPawns | blackKnight | blackBishop | blackRook | blackQueen | blackKing;
    }

    std::uint64_t Board::allPieces() const {
        return whitePieces() | blackPieces();
    }

    void Board::BitBoard::printBoard(Color player) const {
        std::uint64_t pos;
        int p;
        char piece;
        auto getPiece = [&](std::uint64_t pos) ->char {
                if      (whitePawns & pos)  return 'P';
                else if (whiteRook & pos)   return 'R';
                else if (whiteKnight & pos) return 'N';
                else if (whiteBishop & pos) return 'B';
                else if (whiteQueen & pos)  return 'Q';
                else if (whiteKing & pos)   return 'K';
                else if (blackPawns & pos)  return 'p';
                else if (blackRook & pos)   return 'r';
                else if (blackKnight & pos) return 'n';
                else if (blackBishop & pos) return 'b';
                else if (blackQueen & pos)  return 'q';
                else if (blackKing & pos)   return 'k';
                else                        return '.';
        };

        if (Color::White == player)
        {
            for (int rank = 7; rank >= 0; rank--){
                for (int file = 0; file < 8; file++) {
                    p = (rank << 3) + file;
                    pos = 1ULL << p;
                    piece = getPiece(pos);
                    
                    std::cout<<piece<<" ";

                }
                std::cout << "\n";
            }
            std::cout << "\n";
        }
        else if (Color::Black == player)
        {
            for (int rank = 0; rank < 8; rank++){
                for (int file = 0; file < 8; file++) {
                    p = (rank << 3) + file;
                    pos = 1ULL << p;
                    piece = getPiece(pos);
                    
                    std::cout<<piece<<" ";

                }
                std::cout << "\n";
            }
            std::cout << "\n";
        }

    }

    std::uint64_t BitBoard::flipPiece(std::uint64_t b) const {
        b = ((b >> 8) & 0x00FF00FF00FF00FFULL) | ((b & 0x00FF00FF00FF00FFULL) << 8);
        b = ((b >> 16) & 0x0000FFFF0000FFFFULL) | ((b & 0x0000FFFF0000FFFFULL) << 16);
        b = (b >> 32) | (b << 32);
        return b;
    }

}


namespace chess {
    using namespace board;

    Chess::Chess(const std::string& type = "Standard", Color player_color = White, Color tm = White): chessBoard(type), playerColor(player_color), toMove(tm)
    {
        this->play();
    }

    void Chess::play(){
        //
    }
    
}