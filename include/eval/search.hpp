#ifndef SEARCH_H
#define SEARCH_H
#include <cstdint>
#include "../core/board.hpp"
#include "../core/moves.hpp"

namespace eval {

    constexpr int8_t MAX_DEPTH = 15;
    constexpr int8_t MAX_BREADTH = 30;
    constexpr int8_t SELECTIVE_DEPTH = 5;

    class Node{
    private:
        int32_t kingSafety;
        int32_t materialScore;
        int32_t positionalScore;
        int32_t relativeMobility;
        int32_t AttackingScore;
        
    public:
        board::Board position;
        board::Move previousMove;

        Node(int32_t king, int32_t material, int32_t positional, int32_t mobility, int32_t attacking, const board::Board& pos, board::Move previousMove = board::Move(0,0,board::Color::White))
            : kingSafety(king), materialScore(material), positionalScore(positional), relativeMobility(mobility), AttackingScore(attacking), position(pos), previousMove(previousMove) {}

        int32_t getKingSafety() const { return kingSafety; }
        int32_t getMaterialScore() const { return materialScore; }
        int32_t getPositionalScore() const { return positionalScore; }
        int32_t getRelativeMobility() const { return relativeMobility; }
        int32_t getAttackingScore() const { return AttackingScore; }
        board::Move getPreviousMove() const { return previousMove; }

        std::vector<board::Move *> moves;

        friend class evaluator;
    };

    class SearchTree{
    private:
        Node root;

    public:

        SearchTree();
        ~SearchTree();
        Node getRoot() const { return root; }
        Node create_node();
        bool is_terminal(const Node& node);

    };
}





#endif // SEARCH_H