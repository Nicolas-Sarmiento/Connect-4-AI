#ifndef HEURISTICS_HPP_
#define HEURISTICS_HPP_

#include <array>
#include <connect4/board_state.hpp>



inline std::array<int, 42> map_heuristics = {
47, 52, 100, 115, 94, 90, 105,
49, 56, 143, 198,144,149, 129,
 142, 119, 152, 350, 242, 327, 234,
  92, 108, 330, 420, 257, 316, 234,
   205, 218, 219, 206, 492, 179, 151,
    256, 155, 576, 263, 148, 51, 381 
};

struct Weights {
    int three_attack_open;
    int three_attack_semi_open;

    int two_attack_open;
    int two_attack_semi_open;

    
    int three_defense_open;
    int three_defense_semi_open;

    int two_defense_open;
    int two_defense_semi_open;

};



#endif //HEURISTICS_HPP_