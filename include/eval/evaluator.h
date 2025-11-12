#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <cstdint>
#include <vector>
#include "../core/board.h"
#include "../core/moves.h"

namespace eval {
    using namespace board;

    int32_t evaluate_kingSafety(const Board& board);
    int32_t evaluate_material(const Board& board);
    int32_t evaluate_positional(const Board& board);
    int32_t evaluate_mobility(const Board& board);
    int32_t evaluate(const Board& board);

}

#endif // EVALUATOR_H