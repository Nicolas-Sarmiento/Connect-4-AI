#ifndef AGENT_HPP_
#define AGENT_HPP_

#include "connect4/heuristics.hpp"
#include <connect4/board_state.hpp>

enum class Player {
    A, B
};


struct Agent {

    short player;
    Weights heuristic_weights;
    
    Agent( Player player, Weights heuristic_weights );
    
    int find_best_move( BoardState state, int depth );

    int eval_state( BoardState state, short player );

    int alphabeta(BoardState state, int depth, int alpha, int beta, bool isMax);


};


#endif //AGENT_HPP_
