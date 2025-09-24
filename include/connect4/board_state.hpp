#ifndef BOARD_STATE_HPP_
#define BOARD_STATE_HPP_

#include <vector>
#include <string>
#define COLS 7 
#define ROWS 6

constexpr short PLAYER_EMPTY = 0;
constexpr short PLAYER_A = 1;
constexpr short PLAYER_B = 2;
constexpr short TIE = 3;
constexpr short LINE = 4;


struct BoardState{

    std::vector<short> board;

    BoardState();

    bool make_movement(short player, int column);

    short get_value_of_cell(int x, int y); //covnvert from matrix to vector

    bool is_player_winner(short player);

    bool is_tie();

    int count_lines(short player, int line);

    bool match(short p, int r, int c, int dr, int dc, int line_length); 

    void reset();

    std::string board_to_str(); // Don't print it there, just generate the string.

};


#endif //BOARD_STATE_HPP_