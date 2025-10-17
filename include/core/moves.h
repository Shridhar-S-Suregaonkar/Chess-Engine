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

    const std::array<std::pair<uint8_t, uint8_t>, 8> knightMoves = {std::make_pair(2, 1), std::make_pair(1, 2), std::make_pair(-1, 2), std::make_pair(-2, 1), std::make_pair(-2, -1), std::make_pair(-1, -2), std::make_pair(1, -2), std::make_pair(2, -1)};

    const std::array<std::pair<uint8_t, uint8_t>, 8> kingMoves = {std::make_pair(1, 0), std::make_pair(1, 1), std::make_pair(0, 1), std::make_pair(-1, 1), std::make_pair(-1, 0), std::make_pair(-1, -1), std::make_pair(0, -1), std::make_pair(1, -1)};

    class Move {
        public:
            uint8_t from;
            uint8_t to;
            int16_t score = 0;
            bool isCapture = false;
            bool isCheck = false;
            Color color;

            Move(uint8_t f, uint8_t t, Color c);
            Move(uint8_t f, uint8_t t, bool cap, bool check, Color c);
            Move(const Move &m);
    };
}

#endif // PIECES_H