#ifndef SEARCH_H
#define SEARCH_H
#include <cstdint>
#include "../core/board.h"
#include "../core/moves.h"


namespace eval {

    class Node{
    private:
        int32_t kingSafety;
        int32_t materialScore;
        int32_t positionalScore;
        int32_t relativeMobility;
        
    public:
        board::Board position;
        Node(int32_t king, int32_t material, int32_t positional, int32_t mobility, const board::Board& pos)
            : kingSafety(king), materialScore(material), positionalScore(positional), relativeMobility(mobility), position(pos) {}

        int32_t getKingSafety() const { return kingSafety; }
        int32_t getMaterialScore() const { return materialScore; }
        int32_t getPositionalScore() const { return positionalScore; }
        int32_t getRelativeMobility() const { return relativeMobility; }
    };

    class SearchTree{
    private:
        Node root;

    public:

        SearchTree();
        ~SearchTree();
    };
}





#endif // SEARCH_H