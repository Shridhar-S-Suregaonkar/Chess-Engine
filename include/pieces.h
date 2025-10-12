#ifndef PIECES_H
#define PIECES_H
#include <cstdint>
#include <array>


namespace board {

    const std::array<std::pair<uint8_t, uint8_t>, 8> knightMoves = {std::make_pair(2, 1), std::make_pair(1, 2), std::make_pair(-1, 2), std::make_pair(-2, 1), std::make_pair(-2, -1), std::make_pair(-1, -2), std::make_pair(1, -2), std::make_pair(2, -1)};

    const std::array<std::pair<uint8_t, uint8_t>, 8> kingMoves = {std::make_pair(1, 0), std::make_pair(1, 1), std::make_pair(0, 1), std::make_pair(-1, 1), std::make_pair(-1, 0), std::make_pair(-1, -1), std::make_pair(0, -1), std::make_pair(1, -1)};

    class Move {
        public:
            uint8_t from;
            uint8_t to;
            bool isCapture = false;
            bool isCheck = false;

            Move(uint8_t f, uint8_t t) : from(f), to(t) {}
            Move(uint8_t f, uint8_t t, bool cap, bool check) : from(f), to(t), isCapture(cap), isCheck(check) {}
            Move(const Move &m) : from(m.from), to(m.to), isCapture(m.isCapture), isCheck(m.isCheck) {}
    };
}

#endif // PIECES_H