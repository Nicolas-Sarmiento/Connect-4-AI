#include <climits>

#include <connect4/board_state.hpp>
#include <connect4/heuristics.hpp>



int heuristic(BoardState state, short player, Weights w){
    short opponent = player == PLAYER_A ? PLAYER_B : PLAYER_A;
    if ( state.is_player_winner(player) ) return INT_MAX;
    if ( state.is_player_winner(opponent) ) return INT_MIN;

    int player_value, opponent_value;

    player_value = (state.count_lines(player, 3) * w.line_three_attack) + (state.count_lines(player, 2) * w.line_two_attack);

    opponent_value = (state.count_lines(opponent, 3) * w.line_three_defense) + (state.count_lines(opponent, 2) * w.line_two_defense);

    return  player_value - opponent_value;
}

