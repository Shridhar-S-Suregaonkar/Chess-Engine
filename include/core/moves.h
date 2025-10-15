#ifndef PIECES_H
#define PIECES_H
#include <cstdint>
#include <array>


namespace board {

    enum PieceType {
        Pawn, Knight, Bishop, Rook, Queen, King
    };

    enum Color {
        White, Black
    };

    const std::array<std::pair<uint8_t, uint8_t>, 8> knightMoves = {std::make_pair(2, 1), std::make_pair(1, 2), std::make_pair(-1, 2), std::make_pair(-2, 1), std::make_pair(-2, -1), std::make_pair(-1, -2), std::make_pair(1, -2), std::make_pair(2, -1)};

    const std::array<std::pair<uint8_t, uint8_t>, 8> kingMoves = {std::make_pair(1, 0), std::make_pair(1, 1), std::make_pair(0, 1), std::make_pair(-1, 1), std::make_pair(-1, 0), std::make_pair(-1, -1), std::make_pair(0, -1), std::make_pair(1, -1)};

    class Move {
        public:
            uint8_t from;
            uint8_t to;
            int16_t score = 0;
            bool isCapture = false;
            bool isCheck = false;

            Move(uint8_t f, uint8_t t);
            Move(uint8_t f, uint8_t t, bool cap, bool check);
            Move(const Move &m);
    };
}

#endif // PIECES_H