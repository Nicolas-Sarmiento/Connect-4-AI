
#include "connect4/board_state.hpp"
#include <limits.h>
#include <vector>
#include <algorithm>
#include <random>

#include <connect4/heuristics.hpp>
#include <connect4/agent.hpp>


Agent::Agent( Player player, Weights heuristic_weights ){
    if ( player == Player::A) this->player = 1;
    else this->player = 2;

    this->heuristic_weights = heuristic_weights;
}


int Agent::eval_state( BoardState state, short player ){
    short opponent = player == PLAYER_A ? PLAYER_B : PLAYER_A;
    if ( state.is_player_winner(player) ) return INT_MAX;
    if ( state.is_player_winner(opponent) ) return INT_MIN;

    int player_value, opponent_value;

    player_value = (state.count_lines(player, 3) * this->heuristic_weights.line_three_attack) + (state.count_lines(player, 2) * this->heuristic_weights.line_two_attack);

    opponent_value = (state.count_lines(opponent, 3) * this->heuristic_weights.line_three_defense) + (state.count_lines(opponent, 2) * this->heuristic_weights.line_two_defense);

    return  player_value - opponent_value;
}

int random_choice(const std::vector<int> &moves ){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, moves.size() - 1);

    int random_index = dist(gen);
    return moves[random_index];
}

int Agent::alphabeta(BoardState state, int depth, int alpha, int beta, bool isMax){
    if ( depth == 0 || state.is_player_winner(PLAYER_A) || state.is_player_winner(PLAYER_B) )
        return this->eval_state(state, this->player);

    if (isMax){
        int max_eval = INT_MIN;
        bool possible_move;
        int evaluation;
        for ( int move = 0; move < 7; move ++ ){
            BoardState new_state = state;
            possible_move = new_state.make_movement(this->player, move);
            if ( !possible_move ) continue;

            evaluation = alphabeta(new_state, depth-1, alpha, beta, false);
            max_eval = std::max(evaluation, max_eval);
            alpha = std::max(alpha, evaluation);

            if ( beta <= alpha )
                break;

        }
        return  max_eval;

    }else{
        int min_eval = INT_MAX;
        bool possible_move;
        int evaluation;
        short opponent = this->player == PLAYER_A ? PLAYER_B : PLAYER_B;
        for ( int move = 0; move < 7; move ++ ){
            BoardState new_state = state;
            possible_move = new_state.make_movement(opponent, move);
            if ( !possible_move ) continue;

            evaluation = alphabeta(new_state, depth-1, alpha, beta, true);
            min_eval = std::min(evaluation, min_eval);
            beta = std::min(beta, evaluation);

            if ( beta <= alpha )
                break;

        }
        return  min_eval;
    }
    
    return 0;
}

int Agent::find_best_move( BoardState state, int depth ){
    int best_move = -1;
    int best_value = INT_MIN;
    int alpha = INT_MIN;
    int beta = INT_MAX;
    bool possible_move;
    
    std::vector<int> best_values;

    int state_value;
    for ( int move = 0; move < 7; move ++ ){
        BoardState new_state = state;
        possible_move = new_state.make_movement(this->player, move);
        if ( !possible_move ) continue;

        state_value = alphabeta(new_state, depth - 1, alpha, beta, false);

        if ( state_value > best_value ){
            best_value = state_value;

            best_values.clear();
            best_values.push_back(move);

        }else if ( state_value == best_value ){
            best_values.push_back(move);
        }
        
        alpha = std::max(best_value, state_value);

    }
    if (best_values.empty() ) return 0;

    return random_choice(best_values);
}