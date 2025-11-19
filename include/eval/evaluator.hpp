#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <cstdint>
#include <vector>
#include "../core/board.hpp"
#include "../core/moves.hpp"
#include "search.hpp"

namespace eval {
    using namespace board;
    class evaluator {
    public:
        void evaluate(Node & node);
        
    };

}

#endif // EVALUATOR_H