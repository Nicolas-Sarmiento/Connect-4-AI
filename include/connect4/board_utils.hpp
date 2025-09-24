#ifndef BOARD_UTILS_HPP
#define BOARD_UTILS_HPP

#include <vector>

struct LineCounts {
    int open_ended = 0;   
    int semi_open = 0;    
};

LineCounts analyze_lines(const std::vector<short>& board, short player, int length);


#endif // BOARD_UTILS_HPP