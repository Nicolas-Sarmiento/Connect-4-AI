#include <connect4/board_state.hpp>
#include <utility>
#include <vector>


BoardState::BoardState() : board( COLS*ROWS, 0 ) {}


bool BoardState::make_movement(short player, int column){
    if ( player != PLAYER_A && player != PLAYER_B ) return false;
    if( column < 0 || column >= COLS ) return false;
    if (this->board[column] != PLAYER_EMPTY ) return false;
    int final_position = column;
    while ( ((final_position + 7) < 42) && (this->board[final_position + 7] == PLAYER_EMPTY ) ) 
        final_position += 7;

    this->board[final_position] = player;
    return true;
}

short BoardState::get_value_of_cell(int x, int y) {
    if ( ( x < 0 || x >= COLS ) || (  y < 0 || y >= ROWS )) return -1;
    return this->board[(7*y) + x];
}

bool BoardState::match(short p, int r, int c, int dr, int dc, int line_length) {
    for (int i = 0; i < line_length; ++i) {
        int next_r = r + i * dr;
        int next_c = c + i * dc;
        
        
        if (next_r < 0 || next_r >= ROWS || next_c < 0 || next_c >= COLS || get_value_of_cell(next_c, next_r) != p) {
            return false;
        }
    }
    return true; 
}

int BoardState::count_lines(short player, int line) {
    int line_count = 0;
    std::vector<std::pair<int,int>> directions = { {0,1}, {1,0}, {1,1}, {1,-1}}; //( row, column ), movements are: vertical, horizontal, diagonal / , and inverse diagonal
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            for ( std::pair<int, int> p : directions){
                if ( match(player, r, c, p.first, p.second, line)) line_count++;
            }
        }
    }
    return line_count;
}

bool BoardState::is_player_winner(short player) {
    
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            if (match(player, r, c, 0, 1, LINE) ||  match(player, r, c, 1, 0, LINE) ||  match(player, r, c, 1, 1, LINE) || match(player, r, c, 1, -1, LINE)) return true;
        }
    }
    return false; 
}

bool BoardState::is_tie(){
    for(int cell : this->board ){
        if ( cell == PLAYER_EMPTY ) return false;
    }
    return true;
}

std::string BoardState::board_to_str() {
    std::string s;
    for( int i = 0; i < (COLS * ROWS); i++ ){
        if ( !(i % 7) ) s+= '\n';
        s+= '0' + this->board[i];
        s+= ' ';
    }
    return s;
}