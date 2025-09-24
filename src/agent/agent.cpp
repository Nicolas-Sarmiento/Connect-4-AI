
#include "connect4/board_state.hpp"
#include <limits.h>
#include <vector>
#include <algorithm>
#include <random>

#include <connect4/heuristics.hpp>
#include <connect4/agent.hpp>
#include <connect4/board_utils.hpp>

Agent::Agent( Player player, Weights heuristic_weights ){
    if ( player == Player::A) this->player = 1;
    else this->player = 2;

    this->heuristic_weights = heuristic_weights;
}


int Agent::eval_state( BoardState state, short player ){
    short opponent = player == PLAYER_A ? PLAYER_B : PLAYER_A;
    if ( state.is_player_winner(player) ) return INT_MAX;
    if ( state.is_player_winner(opponent) ) return INT_MIN;
    if (state.is_tie()) return 0;

    const std::vector<short>& board = state.board;


    int score = 0;

    LineCounts player_threes = analyze_lines(board, player, 3);
    score += player_threes.open_ended * this->heuristic_weights.three_attack_open;      
    score += player_threes.semi_open * this->heuristic_weights.three_attack_semi_open;        

    LineCounts player_twos = analyze_lines(board, player, 2);
    score += player_twos.open_ended * this->heuristic_weights.two_attack_open;         
    score += player_twos.semi_open * this->heuristic_weights.two_attack_semi_open;           

    
    LineCounts opponent_threes = analyze_lines(board, opponent, 3);
    score -= opponent_threes.open_ended * this->heuristic_weights.three_defense_open;    
    score -= opponent_threes.semi_open * this->heuristic_weights.three_defense_semi_open;
    
    LineCounts opponent_twos = analyze_lines(board, opponent, 2);
    score -= opponent_twos.open_ended * this->heuristic_weights.two_defense_open;
    score -= opponent_twos.semi_open * this->heuristic_weights.two_defense_semi_open;

    if(player == PLAYER_B){
        for( int i = 0; i < (ROWS * COLS); i++ ){
            score += board[i] == this->player ? map_heuristics[i] * 10 : 0;
        }
    }

    return score;
}

int random_choice(const std::vector<int> &moves ){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, moves.size() - 1);

    int random_index = dist(gen);
    return moves[random_index];
}

int Agent::alphabeta( BoardState& state, int depth, int alpha, int beta, bool isMax){
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
        short opponent = this->player == PLAYER_A ? PLAYER_B : PLAYER_A;
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

int Agent::find_best_move( BoardState& state, int depth ){
    int best_move = -1;
    int best_value = INT_MIN;
    int alpha = INT_MIN;
    int beta = INT_MAX;
    bool possible_move;
    
    std::vector<int> best_values;

    BoardState new_state = state;

    for (int i = 0; i < 7 ; i++ ){
        new_state = state;
        possible_move = new_state.make_movement(this->player, i);
        if ( !possible_move ) continue;
        if (new_state.is_player_winner(this->player)) return i;
    }

    int state_value;
    for ( int move = 0; move < 7; move ++ ){
        new_state = state;
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