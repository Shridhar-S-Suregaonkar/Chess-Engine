#ifndef MOVES_H
#define MOVES_H
#include <cstdint>
#include <array>


namespace board {

    enum PieceType: uint8_t {
        Pawn = 0, Knight = 1, Bishop = 2, Rook = 3, Queen = 4, King = 5
    };

    enum Color: uint8_t {
        White = 0, Black = 1
    };

    enum MoveState: uint8_t {
        QuietMove = 0, Capture = 1, Castling = 2, EnPassant = 3, Promotion = 4, PromotionCapture = 5, Check = 6, CheckMate = 7, Illegal = 8
    };

    const std::array<std::pair<uint8_t, uint8_t>, 8> knightMoves = {std::make_pair(2, 1), std::make_pair(1, 2), std::make_pair(-1, 2), std::make_pair(-2, 1), std::make_pair(-2, -1), std::make_pair(-1, -2), std::make_pair(1, -2), std::make_pair(2, -1)};

    const std::array<std::pair<uint8_t, uint8_t>, 8> kingMoves = {std::make_pair(1, 0), std::make_pair(1, 1), std::make_pair(0, 1), std::make_pair(-1, 1), std::make_pair(-1, 0), std::make_pair(-1, -1), std::make_pair(0, -1), std::make_pair(1, -1)};

    class Move {
        public:
            uint8_t from;
            uint8_t to;
            int16_t score = 0;
            MoveState state;
            Color color;
            PieceType piece;

            Move(uint8_t f, uint8_t t, Color c);
            Move(uint8_t f, uint8_t t, MoveState s, Color c);
            Move(const Move &m);
    };

    std::vector<Move> movesEvaluator(const Board& board, Color toMove);
}

#endif // PIECES_H