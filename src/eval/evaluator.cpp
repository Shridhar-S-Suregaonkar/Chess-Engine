
#include "../include/eval/evaluator.hpp"
#include "../include/core/header.hpp"
#include "../include/core/moves.hpp"
#include "../include/core/board.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <vector>

namespace eval {
    
    void evaluator::evaluate(Node & node) {
        node.materialScore =  PAWN_VALUE * (__builtin_popcountll(node.position.whitePawns) - __builtin_popcountll(node.position.blackPawns))
                            + KNIGHT_VALUE * (__builtin_popcountll(node.position.whiteKnight) - __builtin_popcountll(node.position.blackKnight))
                            + BISHOP_VALUE * (__builtin_popcountll(node.position.whiteBishop) - __builtin_popcountll(node.position.blackBishop))
                            + ROOK_VALUE * (__builtin_popcountll(node.position.whiteRook) - __builtin_popcountll(node.position.blackRook))
                            + QUEEN_VALUE * (__builtin_popcountll(node.position.whiteQueen) - __builtin_popcountll(node.position.blackQueen));
        
    }

}