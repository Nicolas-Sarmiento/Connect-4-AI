#include "connect4/board_state.hpp"
#include "connect4/heuristics.hpp"
#include <iostream>
#include <connect4/agent.hpp>
#include <connect4/game.hpp>


Weights r = { 
.three_attack_open = 5000 ,
.three_attack_semi_open = 500 ,
.two_attack_open = 1000 ,
.two_attack_semi_open = 100 ,
.three_defense_open = 6000,
.three_defense_semi_open = 6000,
.two_defense_open = 1000,
.two_defense_semi_open = 500
};
Weights y = { 
    .three_attack_open = 5000 ,
    .three_attack_semi_open = 500 ,
    .two_attack_open = 1000 ,
    .two_attack_semi_open = 100 ,
    .three_defense_open = 6000,
    .three_defense_semi_open = 6000,
    .two_defense_open = 1000,
    .two_defense_semi_open = 500
};


int DEPTH_A = 5;
int DEPTH_B = 6;


inline void print_board(short player, BoardState board ){
  std::cout << "-------------" << '\n';
  std::cout << " turn : " << (player == PLAYER_A ? "Red" : "Yellow" ) << '\n';
  std::cout << "-------------" << '\n';
  std::cout << "1 2 3 4 5 6 7\n";
  std::cout << board.board_to_str() << '\n';
  std::cout << "=============" << '\n';
}

inline int play_game(Agent red, Agent yellow, int depthA, int depthB){
  BoardState board;
  short player;
  int move;
  bool is_game_over = false, turn = true;
  int result;
  while( !is_game_over ){
    player = turn? PLAYER_A : PLAYER_B;
    if ( turn ) {
      move = red.find_best_move(board, depthA);
    }else {
      move = yellow.find_best_move(board, depthB);
      // std::cout << "Your move -> (1-7): ";
      // std::cin >> move; move--;
    }
    board.make_movement(player, move);

    //print_board(player, board);

    turn = !turn;
    is_game_over = board.is_player_winner(player);
    if (is_game_over) {
      if ( player == PLAYER_A ) result =  1;
      else{
        for( int i = 0; i < (COLS * ROWS); i++ ){
          map_heuristics[i] += board.board[i] == PLAYER_B ? 1 : 0;
        }
        result =  -1;
      }
    }
    if( board.is_tie() ){
      break;
      result = 0;
    }
  }
  return result;
}



void terminal_simulation(){
    int n; 
    std::cout << "Number of games: ";
    std::cin >> n;


    Agent red (Player::A, r);
    Agent yellow (Player::B, y); 

  
    int yellow_games = 0, red_games = 0, result;
    
    for (int i = 0; i < n; i++ ){
        std::cout << "Game " << i << '\n';
        result = play_game(red, yellow, DEPTH_A, DEPTH_B);
        if (result > 0 ) red_games++;
        else if ( result < 0) yellow_games++;
    }

    std::cout << "yellow: " << yellow_games << '\n';
    std::cout << "red: " << red_games << '\n';
    for(int i : map_heuristics ){
      std::cout << i << ' ';
    }
    std::cout << '\n';
}