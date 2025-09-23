#ifndef AGENT_HPP_
#define AGENT_HPP_

#include <functional>

#include <connect4/board_state.hpp>

enum class Player {
    A, B
};


struct Agent {

    short player;
    std::function<int(BoardState, short)> eval_state;

    Agent( Player player, std::function<int(BoardState, short)> heuristic );

    int find_best_move( BoardState state, int depth );

    int alphabeta(BoardState state, int depth, int alpha, int beta, bool isMax);


};


#endif //AGENT_HPP_
