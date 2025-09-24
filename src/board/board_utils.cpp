#include <connect4/board_utils.hpp>
#include <connect4/board_state.hpp>

void check_direction(const std::vector<short>& board, short player, int length,int r, int c, int dr, int dc, LineCounts& counts) {
    
    
    for (int i = 0; i < length; ++i) {
        int current_r = r + i * dr;
        int current_c = c + i * dc;
        if (current_r < 0 || current_r >= ROWS || current_c < 0 || current_c >= COLS) {
            return;
        }
        if (board[current_r * COLS + current_c] != player) {
            return; 
        }
    }
    
    int open_ends = 0;
    
    int before_r = r - dr;
    int before_c = c - dc;
    if (before_r >= 0 && before_r < ROWS && before_c >= 0 && before_c < COLS && board[before_r * COLS + before_c] == PLAYER_EMPTY)  open_ends++;


    
    int after_r = r + length * dr;
    int after_c = c + length * dc;
    if (after_r >= 0 && after_r < ROWS && after_c >= 0 && after_c < COLS && board[after_r * COLS + after_c] == PLAYER_EMPTY)  open_ends++;

    
    if (open_ends == 2) {
        counts.open_ended++;
    } else if (open_ends == 1) {
        counts.semi_open++;
    }
}



LineCounts analyze_lines(const std::vector<short>& board, short player, int length) {
    LineCounts counts;

    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            
            check_direction(board, player, length, r, c, 0, 1, counts);  
            check_direction(board, player, length, r, c, 1, 0, counts);  
            check_direction(board, player, length, r, c, 1, 1, counts);  
            check_direction(board, player, length, r, c, 1, -1, counts); 
        }
    }
    return counts;
}

