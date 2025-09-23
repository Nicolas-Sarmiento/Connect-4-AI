#ifndef HEURISTICS_HPP_
#define HEURISTICS_HPP_

#include <connect4/board_state.hpp>

struct Weights {
    int line_three_attack;
    int line_two_attack;
    
    int line_three_defense;
    int line_two_defense;
};



#endif //HEURISTICS_HPP_