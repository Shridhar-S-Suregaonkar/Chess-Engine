#include <iostream>
#include <vector>
#include <utility>

#include "board.hpp"
#include "header.hpp"
#include "moves.hpp"

namespace board {

    Move::Move(uint8_t f, uint8_t t, MoveState s = MoveState::QuietMove, Color c) : from(f), to(t), state(s), color(c) {}
    Move::Move(const Move &m) : from(m.from), to(m.to), state(m.state), score(m.score), color(m.color) {}

    std::vector<Move> Board::whitePawnQuietMoves() const {
        std::vector<Move> moves;
        std::uint64_t occupied = allPieces();
        std::uint64_t single_push = (whitePawns << 8) & ~occupied;
        std::uint64_t double_push = ((single_push & RANK_3) << 8) & ~occupied;
        
        while(single_push){
            int8_t i = static_cast<int8_t>(__builtin_ctzll(single_push));
            moves.emplace_back(i - 8, i, MoveState::QuietMove, Color::White);
            single_push &= single_push - 1;
        }
            
        while(double_push){
            int8_t i = static_cast<int8_t>(__builtin_ctzll(double_push));
            moves.emplace_back(i - 16, i, MoveState::QuietMove, Color::White);
            double_push &= double_push - 1;
        }
        
        return moves;
    }

    std::vector<Move> Board::whitePawnCaptures() const {
        std::vector<Move> moves;
        std::uint64_t black_occupied = blackPieces();
        std::uint64_t leftCaptures = ((whitePawns & ~FILE_A) << 7) & black_occupied;
        std::uint64_t rightCaptures = ((whitePawns & ~FILE_H) << 9) & black_occupied;

        while(leftCaptures){
            int8_t i = static_cast<int8_t>(__builtin_ctzll(leftCaptures));
            moves.emplace_back(i - 7, i, MoveState::Capture, Color::White);
            leftCaptures &= leftCaptures - 1;
        }
        while(rightCaptures){
            int8_t i = static_cast<int8_t>(__builtin_ctzll(rightCaptures));
            moves.emplace_back(i - 9, i, MoveState::Capture, Color::White);
            rightCaptures &= rightCaptures - 1;
        }
        return moves;
    }

    std::vector<Move> Board::blackPawnQuietMoves() const {
        std::vector<Move> moves;
        std::uint64_t occupied = allPieces();
        std::uint64_t single_push = (blackPawns >> 8) & ~occupied;
        std::uint64_t double_push = ((single_push & RANK_6) >> 8) & ~occupied;

        while(single_push){
            int8_t i = static_cast<int8_t>(__builtin_ctzll(single_push));
            moves.emplace_back(i + 8, i, MoveState::QuietMove, Color::Black);
            single_push &= single_push - 1;
        }
        while(double_push){
            int8_t i = static_cast<int8_t>(__builtin_ctzll(double_push));
            moves.emplace_back(i + 16, i, MoveState::QuietMove, Color::Black);
            double_push &= double_push - 1;
        }
        return moves;
    }

    std::vector<Move> Board::blackPawnCaptures() const {
        std::vector<Move> moves;
        std::uint64_t white_occupied = whitePieces();
        std::uint64_t leftCaptures = ((blackPawns & ~FILE_H) >> 7) & white_occupied;
        std::uint64_t rightCaptures = ((blackPawns & ~FILE_A) >> 9) & white_occupied;

        while(leftCaptures){
            int8_t i = static_cast<int8_t>(__builtin_ctzll(leftCaptures));
            moves.emplace_back(i + 7, i, MoveState::Capture, Color::Black);
            leftCaptures &= leftCaptures - 1;
        }

        while(rightCaptures){
            int8_t i = static_cast<int8_t>(__builtin_ctzll(rightCaptures));
            moves.emplace_back(i + 9, i, MoveState::Capture, Color::Black);
            rightCaptures &= rightCaptures - 1;
        }
        return moves;
    }

    std::vector<Move> Board::whiteKnightMoves() const {
        std::vector<Move> moves;
        std::uint64_t occupied = whitePieces();
        uint64_t opponent = blackPieces();
        uint64_t knights = whiteKnight;
        while(knights) {
            int8_t sq = static_cast<int8_t>(__builtin_ctzll(knights));
            int8_t rank = sq / 8;
            int8_t file = sq % 8;
            for (int8_t d = 0; d < 8; d++) {
                int8_t fto_i = file + knightMoves[d].first;
                int8_t rto_i = rank + knightMoves[d].second;
                if (rto_i >= 0 && rto_i < 8 && fto_i >= 0 && fto_i < 8) {
                    int8_t to = rto_i * 8 + fto_i;
                    if (opponent & (1ULL << to)) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::White);
                    } else
                    if (!(occupied & (1ULL << to))) {
                        moves.emplace_back(sq, to, MoveState::QuietMove, Color::White);
                    }
                }
            }
            knights &= ~ (1ULL << sq);
        }
        return moves;
    }

    std::vector<Move> Board::blackKnightMoves() const {
        std::vector<Move> moves;
        std::uint64_t occupied = blackPieces();
        uint64_t opponent = whitePieces();
        uint64_t knights = blackKnight;
        while(knights) {
            int8_t sq = static_cast<int8_t>(__builtin_ctzll(knights));
            int8_t rank = sq / 8;
            int8_t file = sq % 8;
            for (int8_t d = 7; d >= 0; d--) {
                int8_t fto_i = file + knightMoves[d].first;
                int8_t rto_i = rank + knightMoves[d].second;
                if (rto_i >= 0 && rto_i < 8 && fto_i >= 0 && fto_i < 8) {
                    int8_t to = rto_i * 8 + fto_i;
                    if (opponent & (1ULL << to)) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::Black);
                    } else
                    if (!(occupied & (1ULL << to))) {
                        moves.emplace_back(sq, to, MoveState::QuietMove, Color::Black);
                    }
                }
            }
            knights &= ~ (1ULL << sq);
        }
        return moves;
    }

    std::vector<Move> Board::whiteBishopMoves() const {
        std::vector<Move> moves;
        std::uint64_t occupied = allPieces();
        std::uint64_t opponent = blackPieces();
        uint64_t bishops = whiteBishop;
        while (bishops) {
            int8_t sq = static_cast<int8_t>(__builtin_ctzll(bishops));
            int8_t rank = sq / 8;
            int8_t file = sq % 8;
            int8_t dist;
            int8_t rto_i, fto_i;
            // Up-left
            for (dist = 1; ; dist++) {
                rto_i = rank + dist;
                fto_i = file - dist;
                if (rto_i >= 8 || fto_i < 0) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::White);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::White);
                }
            }
            // Up-right
            for (dist = 1; ; dist++) {
                rto_i = rank + dist;
                fto_i = file + dist;
                if (rto_i >= 8 || fto_i >= 8) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::White);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::White);
                }
            }
            // Down-left
            for (dist = 1; ; dist++) {
                rto_i = rank - dist;
                fto_i = file - dist;
                if (rto_i < 0 || fto_i < 0) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::White);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::White);
                }
            }
            // Down-right
            for (dist = 1; ; dist++) {
                rto_i = rank - dist;
                fto_i = file + dist;
                if (rto_i < 0 || fto_i >= 8) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::White);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::White);
                }
            }
            bishops &= ~ (1ULL << sq);
        }
        return moves;
    }

    std::vector<Move> Board::blackBishopMoves() const {
        std::vector<Move> moves;
        std::uint64_t occupied = allPieces();
        std::uint64_t opponent = whitePieces();
        uint64_t bishops = blackBishop;
        while(bishops){
            int8_t sq = static_cast<int8_t>(__builtin_ctzll(bishops));
            int8_t rank = sq / 8;
            int8_t file = sq % 8;
            int8_t dist;
            int8_t rto_i, fto_i;
            // Up-left
            for (dist = 1; ; dist++) {
                rto_i = rank + dist;
                fto_i = file - dist;
                if (rto_i >= 8 || fto_i < 0) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::Black);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::Black);
                }
            }
            // Up-right
            for (dist = 1; ; dist++) {
                rto_i = rank + dist;
                fto_i = file + dist;
                if (rto_i >= 8 || fto_i >= 8) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::Black);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::Black);
                }
            }
            // Down-left
            for (dist = 1; ; dist++) {
                rto_i = rank - dist;
                fto_i = file - dist;
                if (rto_i < 0 || fto_i < 0) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::Black);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::Black);
                }
            }
            // Down-right
            for (dist = 1; ; dist++) {
                rto_i = rank - dist;
                fto_i = file + dist;
                if (rto_i < 0 || fto_i >= 8) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::Black);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::Black);
                }
            }
            bishops &= ~ (1ULL << sq);
        }
        return moves;
    }

    std::vector<Move> Board::whiteRookMoves() const {
        std::vector<Move> moves;
        std::uint64_t occupied = allPieces();
        std::uint64_t opponent = blackPieces();
        uint64_t rooks = whiteRook;
        while (rooks) {
            int8_t sq = static_cast<int8_t>(__builtin_ctzll(rooks));
            int8_t rank = sq / 8;
            int8_t file = sq % 8;
            int8_t dist;
            int8_t rto_i, fto_i;
            // Up
            for (dist = 1; ; dist++) {
                int8_t rto_i = rank + dist;
                if (rto_i >= 8) break;
                int8_t to = rto_i * 8 + file;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::White);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::White);
                }
            }
            // Down
            for (dist = 1; ; dist++) {
                rto_i = rank - dist;
                if (rto_i < 0) break;
                int8_t to = rto_i * 8 + file;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::White);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::White);
                }
            }
            // Right
            for (dist = 1; ; dist++) {
                fto_i = file + dist;
                if (fto_i >= 8) break;
                int8_t to = rank * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::White);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::White);
                }
            }
            // Left
            for (dist = 1; ; dist++) {
                fto_i = file - dist;
                if (fto_i < 0) break;
                int8_t to = rank * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::White);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::White);
                }
            }
            rooks &= ~ (1ULL << sq);
        }
        return moves;
    }

    std::vector<Move> Board::blackRookMoves() const {
        std::vector<Move> moves;
        std::uint64_t occupied = allPieces();
        std::uint64_t opponent = whitePieces();
        uint64_t rooks = blackRook;
        while (rooks) {
            int8_t sq = static_cast<int8_t>(__builtin_ctzll(rooks));
            int8_t rank = sq / 8;
            int8_t file = sq % 8;
            int8_t dist;
            int8_t rto_i, fto_i;
            // Up
            for (dist = 1; ; dist++) {
                rto_i = rank + dist;
                if (rto_i >= 8) break;
                int8_t to = rto_i * 8 + file;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::Black);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::Black);
                }
            }
            // Down
            for (dist = 1; ; dist++) {
                rto_i = rank - dist;
                fto_i = file;
                if (rto_i < 0) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::Black);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::Black);
                }
            }
            // Right
            for (dist = 1; ; dist++) {
                rto_i = rank;
                fto_i = file + dist;
                if (fto_i >= 8) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::Black);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::Black);
                }
            }
            // Left
            for (dist = 1; ; dist++) {
                rto_i = rank;
                fto_i = file - dist;
                if (fto_i < 0) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::Black);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::Black);
                }
            }
            rooks &= ~ (1ULL << sq);
        }
        return moves;
    }

    std::vector<Move> Board::whiteQueenMoves() const {
        std::vector<Move> moves;
        std::uint64_t occupied = allPieces();
        std::uint64_t opponent = blackPieces();
        uint64_t queens = whiteQueen;

        while (queens) {
            int8_t sq = static_cast<int8_t>(__builtin_ctzll(queens));
            int8_t rank = sq / 8;
            int8_t file = sq % 8;
            int8_t rto_i, fto_i;
            int8_t dist;

            // Up
            for (dist = 1; ; dist++) {
                rto_i = rank + dist;
                fto_i = file;
                if (rto_i >= 8) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::White);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::White);
                }
            }
            // Down
            for (dist = 1; ; dist++) {
                rto_i = rank - dist;
                fto_i = file;
                if (rto_i < 0) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::White);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::White);
                }
            }
            // Right
            for (dist = 1; ; dist++) {
                rto_i = rank;
                fto_i = file + dist;
                if (fto_i >= 8) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::White);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::White);
                }
            }
            // Left
            for (dist = 1; ; dist++) {
                rto_i = rank;
                fto_i = file - dist;
                if (fto_i < 0) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::White);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::White);
                }
            }
            // Bishop directions
            // Up-left
            for (dist = 1; ; dist++) {
                rto_i = rank + dist;
                fto_i = file - dist;
                if (rto_i >= 8 || fto_i < 0) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::White);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::White);
                }
            }
            // Up-right
            for (dist = 1; ; dist++) {
                rto_i = rank + dist;
                fto_i = file + dist;
                if (rto_i >= 8 || fto_i >= 8) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::White);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::White);
                }
            }
            // Down-left
            for (dist = 1; ; dist++) {
                rto_i = rank - dist;
                fto_i = file - dist;
                if (rto_i < 0 || fto_i < 0) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::White);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::White);
                }
            }
            // Down-right
            for (dist = 1; ; dist++) {
                rto_i = rank - dist;
                fto_i = file + dist;
                if (rto_i < 0 || fto_i >= 8) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::White);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::White);
                }
            }
        }
        return moves;
    }

    std::vector<Move> Board::blackQueenMoves() const {
        std::vector<Move> moves;
        std::uint64_t occupied = allPieces();
        std::uint64_t opponent = whitePieces();
        uint64_t queens = blackQueen;

        while (queens) {
            int8_t sq = static_cast<int8_t>(__builtin_ctzll(queens));
            queens &= queens - 1;
            int8_t rank = sq / 8;
            int8_t file = sq % 8;
            int8_t rto_i, fto_i;
            int8_t dist;
            // Rook directions
            // Up
            for (dist = 1; ; dist++) {
                rto_i = rank + dist;
                fto_i = file;
                if (rto_i >= 8) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::Black);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::Black);
                }
            }
            // Down
            for (dist = 1; ; dist++) {
                rto_i = rank - dist;
                fto_i = file;
                if (rto_i < 0) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::Black);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::Black);
                }
            }
            // Right
            for (dist = 1; ; dist++) {
                rto_i = rank;
                fto_i = file + dist;
                if (fto_i >= 8) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::Black);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::Black);
                }
            }
            // Left
            for (dist = 1; ; dist++) {
                rto_i = rank;
                fto_i = file - dist;
                if (fto_i < 0) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::Black);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::Black);
                }
            }
            // Bishop directions
            // Up-left
            for (dist = 1; ; dist++) {
                rto_i = rank + dist;
                fto_i = file - dist;
                if (rto_i >= 8 || fto_i < 0) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::Black);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::Black);
                }
            }
            // Up-right
            for (dist = 1; ; dist++) {
                rto_i = rank + dist;
                fto_i = file + dist;
                if (rto_i >= 8 || fto_i >= 8) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::Black);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::Black);
                }
            }
            // Down-left
            for (dist = 1; ; dist++) {
                rto_i = rank - dist;
                fto_i = file - dist;
                if (rto_i < 0 || fto_i < 0) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::Black);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::Black);
                }
            }
            // Down-right
            for (dist = 1; ; dist++) {
                rto_i = rank - dist;
                fto_i = file + dist;
                if (rto_i < 0 || fto_i >= 8) break;
                int8_t to = rto_i * 8 + fto_i;
                std::uint64_t tpos = 1ULL << to;
                if (occupied & tpos) {
                    if (opponent & tpos) {
                        moves.emplace_back(sq, to, MoveState::Capture, Color::Black);
                    }
                    break;
                } else {
                    moves.emplace_back(sq, to, MoveState::QuietMove, Color::Black);
                }
            }
        }
        return moves;
    }

    std::vector<Move> Board::whiteKingMoves() const {
        std::vector<Move> moves;
        int8_t sq = static_cast<int8_t>(__builtin_ctzll(whiteKing));

        int8_t rank = sq / 8;
        int8_t file = sq % 8;
        int8_t rto_i, fto_i;
        for (int8_t d = 0; d < 8; d++) {
            rto_i = rank + kingMoves[d].first;
            fto_i = file + kingMoves[d].second;
            if (rto_i >= 0 && rto_i < 8 && fto_i >= 0 && fto_i < 8) {
                int8_t to = rto_i * 8 + fto_i;
                if (!(whitePieces() & (1ULL << to))) {
                    moves.emplace_back(sq, to);
                }
            }
        }
        return moves;
    }

    std::vector<Move> Board::blackKingMoves() const {
        std::vector<Move> moves;

        int8_t sq = static_cast<int8_t>(__builtin_ctzll(blackKing));
        int8_t rank = sq / 8;
        int8_t rto_i, fto_i;
        int8_t file = sq % 8;
        for (int8_t d = 0; d < 8; d++) {
            rto_i = rank + kingMoves[d].first;
            fto_i = file + kingMoves[d].second;
            if (rto_i >= 0 && rto_i < 8 && fto_i >= 0 && fto_i < 8) {
                int8_t to = rto_i * 8 + fto_i;
                if (!(blackPieces() & (1ULL << to))) {
                    moves.emplace_back(sq, to);
                }
            }
        }
        return moves;
    }
    
}
