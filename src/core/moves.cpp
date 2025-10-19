#include <iostream>
#include <vector>
#include <utility>

#include "board.h"
#include "header.h"
#include "moves.h"

namespace board {

    Move::Move(uint8_t f, uint8_t t, Color c) : from(f), to(t), color(c) {}
    Move::Move(uint8_t f, uint8_t t, MoveState s, Color c) : from(f), to(t), state(s), color(c) {}
    Move::Move(const Move &m) : from(m.from), to(m.to), state(m.state), score(m.score), color(m.color) {}

    std::vector<Move> Board::whitePawnQuietMoves() const {
        std::vector<Move> moves;
        uint64_t occupied = allPieces();
        uint64_t single_push = (whitePawns << 8) & ~occupied;
        uint64_t double_push = ((single_push & RANK_3) << 8) & ~occupied;
        for (uint8_t i = 0; i < 64; i++) {
            if (single_push & (1ULL << i)) {
                moves.emplace_back(i - 8, i);
            }
            if (double_push & (1ULL << i)) {
                moves.emplace_back(i - 16, i);
            }
        }
        return moves;
    }

    std::vector<Move> Board::whitePawnCaptures() const {
        std::vector<Move> moves;
        uint64_t black_occupied = blackPieces();
        uint64_t leftCaptures = ((whitePawns & ~FILE_A) << 7) & black_occupied;
        uint64_t rightCaptures = ((whitePawns & ~FILE_H) << 9) & black_occupied;

        for (uint8_t i = 0; i < 64; i++) {
            if (leftCaptures & (1ULL << i)) {
                moves.emplace_back(i - 7, i);
            }
            if (rightCaptures & (1ULL << i)) {
                moves.emplace_back(i - 9, i);
            }
        }
        return moves;
    }

    std::vector<Move> Board::blackPawnQuietMoves() const {
        std::vector<Move> moves;
        uint64_t occupied = allPieces();
        uint64_t single_push = (blackPawns >> 8) & ~occupied;
        uint64_t double_push = ((single_push & RANK_6) >> 8) & ~occupied;
        for (uint8_t i = 0; i < 64; i++) {
            if (single_push & (1ULL << i)) {
                moves.emplace_back(i + 8, i);
            }
            if (double_push & (1ULL << i)) {
                moves.emplace_back(i + 16, i);
            }
        }
        return moves;
    }

    std::vector<Move> Board::blackPawnCaptures() const {
        std::vector<Move> moves;
        uint64_t white_occupied = whitePieces();
        uint64_t leftCaptures = ((blackPawns & ~FILE_H) >> 7) & white_occupied;
        uint64_t rightCaptures = ((blackPawns & ~FILE_A) >> 9) & white_occupied;
        for (uint8_t i = 0; i < 64; i++) {
            if (leftCaptures & (1ULL << i)) {
                moves.emplace_back(i + 7, i);
            }
            if (rightCaptures & (1ULL << i)) {
                moves.emplace_back(i + 9, i);
            }
        }
        return moves;
    }

    std::vector<Move> Board::whiteKnightMoves() const {
        std::vector<Move> moves;
        uint64_t occupied = allPieces();
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
                        moves.emplace_back(sq, to);
                    }
                }
            }
        }
        return moves;
    }

    std::vector<Move> Board::blackKnightMoves() const {
        std::vector<Move> moves;
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
                        moves.emplace_back(sq, to);
                    }
                }
            }
        }
        return moves;
    }

    std::vector<Move> Board::whiteBishopMoves() const {
        std::vector<Move> moves;
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
                }
            }
        }
        return moves;
    }

    std::vector<Move> Board::blackBishopMoves() const {
        std::vector<Move> moves;
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
                }
            }
        }
        return moves;
    }

    std::vector<Move> Board::whiteRookMoves() const {
        std::vector<Move> moves;
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
                }
            }
        }
        return moves;
    }

    std::vector<Move> Board::blackRookMoves() const {
        std::vector<Move> moves;
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
                }
            }
        }
        return moves;
    }

    std::vector<Move> Board::whiteQueenMoves() const {
        std::vector<Move> moves;
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
                }
            }
        }
        return moves;
    }

    std::vector<Move> Board::blackQueenMoves() const {
        std::vector<Move> moves;
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
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
                        moves.emplace_back(sq, to);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to);
                }
            }
        }
        return moves;
    }

    std::vector<Move> Board::whiteKingMoves() const {
        std::vector<Move> moves;
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
                        moves.emplace_back(sq, to);
                    }
                }
            }
        }
        return moves;
    }

    std::vector<Move> Board::blackKingMoves() const {
        std::vector<Move> moves;
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
                        moves.emplace_back(sq, to);
                    }
                }
            }
        }
        return moves;
    }
    
}
